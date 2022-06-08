#include "functions.h"

// Server descriptor
int sd;

void signal_handler(int signum)
{
    printf("\nClosing server...\n");
    if (close(sd) == -1)
    {
        perror("Can't close the socket");
        exit(1);
    }
    exit(0);
}

int createSocket()
{
    // Socket creation with UDP specifications
    int result = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (result == -1)
    {
        perror("Can't create socket");
        exit(1);
    }
    return result;
}

bool isNumber(char input)
{
    // Checking if a character is a number
    if (input > 57 || input < 48)
    {
        return false;
    }
    return true;
}

bool checkInput(char *data, int length, bool *isRN)
{
    for (int i = 0; i < length; i++)
    {
        if (data[i] == ' ')
        {
            continue;
        }
        if (data[i] == '\n')
        {
            *isRN = true;
            continue;
        }
        if (!isNumber(data[i]))
        {
            return false;
        }
    }
    return true;
}

unsigned long long int performAction(char *data, int length)
{
    unsigned long long int number = 0;
    unsigned long long int result = 0;

    char *numberStr = strtok(data, " ");

    while (numberStr != NULL)
    {
        number = strtoull(numberStr, NULL, 10);
        if (number == ULLONG_MAX && errno == ERANGE)
        {
            return -1;
        }
        if (number > INT_MAX - result)
        {
            return -1;
        }
        result += number;
        numberStr = strtok(NULL, " ");
    }

    return result;
}

void addEnding(char *data)
{
    strcat(data, "\r\n");
}

int sendError(int sd, char *msg, size_t msg_len, struct sockaddr *clientAddr, socklen_t len)
{
    int bytesToSent = sendto(sd, msg, msg_len, MSG_CONFIRM, clientAddr, len);
    if (bytesToSent == -1)
    {
        perror("Can't send an error");
        exit(1);
    }
    return bytesToSent;
}

void prepare_sockaddr(struct sockaddr_in6 *addr, int port)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin6_family = AF_INET6;
    addr->sin6_addr = in6addr_any;
    addr->sin6_port = htons(port);
}

void startSumServer(int port)
{
    // Creating socket
    sd = createSocket();

    // Main buffer
    char buff[MAX_LINE];

    // Variable for state if incoming message has \r\n in the end of line.
    bool isRN;

    // Buffer for the server answer (size = 12,
    // because of space that maximum UINT number can have + space for \r\n)
    char answer[12];

    // bytesFromClient - number of bytes received form client
    // bytesToSend - number of bytes sent to a client
    // result - result of server action
    int bytesFromClient, bytesToSent;

    unsigned long long int result;

    // result in char* type
    char result_char[10];

    // Error messages
    char errorMsg[9] = "ERROR";

    // Structures for server and client
    struct sockaddr_in6 server_addr, client_addr;

    // Filling server structure
    prepare_sockaddr(&server_addr, port);

    // memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = INADDR_ANY;
    // server_addr.sin_port = htons(port);

    // Binding socket to the address
    if (bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Can't bind the socket");
        exit(1);
    }

    socklen_t len = sizeof(client_addr);

    // Main loop
    while (true)
    {

        // Clearing buffers and variables
        memset(answer, 0, 12);
        memset(buff, 0, MAX_LINE);
        memset(result_char, 0, 10);
        isRN = false;

        // Receiving data and checking it
        bytesFromClient = recvfrom(sd, buff, MAX_LINE, MSG_WAITALL, (struct sockaddr *)&client_addr, &len);
        if (bytesFromClient == -1)
        {
            perror("Can't receive data");
            exit(1);
        }

        // Checking input if it's readable by server
        if (checkInput(buff, bytesFromClient, &isRN))
        {
            printf("\nData from client: %s", buff);
        }
        else
        {
            // If not readable informing client
            bytesToSent = sendError(sd, errorMsg, strlen(errorMsg), (struct sockaddr *)&client_addr, len);
            printf("\nUnable to read data from client\n");
            continue;
        }

        // Getting result
        result = performAction(buff, bytesFromClient - 1);

        if (result == -1)
        {
            // If data is too big server informs client that this data provokes overflow
            printf("\nLarge data has come from client and overflow happen");
            bytesToSent = sendError(sd, errorMsg, strlen(errorMsg), (struct sockaddr *)&client_addr, len);
            continue;
        }

        // Printing result
        printf("Result: %lld\n", result);

        // Converting result from int to char*
        bytesToSent = sprintf(answer, "%lld", result);
        if (bytesToSent == -1)
        {
            perror("Can't convert data");
            // Informing client that error occurred
            bytesToSent = sendError(sd, errorMsg, strlen(errorMsg), (struct sockaddr *)&client_addr, len);
            continue;
        }

        // Preparing answer
        if (isRN)
        {
            addEnding(answer);
        }

        // Sending answer
        bytesToSent = sendto(sd, answer, strlen(answer), MSG_CONFIRM, (struct sockaddr *)&client_addr, len);
        if (bytesToSent == -1)
        {
            perror("Can't send a message");
        }
        else
        {
            printf("\n - Answer was sent successfully...\n");
        }
    }
}