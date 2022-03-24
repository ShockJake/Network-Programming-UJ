#include "functions.h"

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
    int result = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (result == -1)
    {
        perror("Can't create socket");
        exit(1);
    }
    return result;
}

bool isNumber(char input)
{
    if (input > 57 || input < 48)
    {
        return false;
    }
    return true;
}

bool checkInput(char *data, int lenght)
{
    for (int i = 0; i < lenght; i++)
    {
        if (data[i] == ' ')
        {
            continue;
        }
        if (data[i] == '\n')
        {
            continue;
        }
        if (data[i] == '\r')
        {
            continue;
        }
        if (!isNumber(data[i]))
        {
            return false;
        }
    }
    return true;
}

int performAction(char *data, int lenght)
{
    int number = 0;
    int result = 0;
    for (int i = 0; i < lenght; i++)
    {
        if (data[i] == ' ')
        {
            continue;
        }

        number = (int)data[i] - (int)'0';

        if (number > UINT_MAX - result)
        {
            return -1;
        }
        result += number;
    }
    return result;
}

void addEnding(char *data, bool type)
{
    if (type)
    {
        strcat(data, "\r\n");
    }
    else
    {
        strcat(data, "\n");
    }
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

void startSumServer(int port)
{
    // Creating socket
    sd = createSocket();

    // Main buffer
    char buff[MAXLINE];

    // Variable for state if incoming message has \r\n in the end of line.
    bool isRN = false;

    // Buffer for the server answer (size = 12,
    // because of space that maximum UINT number can have + space for \r\n)
    char answer[12];

    // bytesFromClient - number of bytes recieved form client
    // bytesToSend - number of byted sent to a client
    // result - result of server action
    int bytesFromClient, bytesToSent, result;

    // result in char* type
    char result_char[10];

    // Error messages
    char errorMsg[9] = "ERROR";

    // Structures for server and client
    struct sockaddr_in server_addr, client_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Filling server structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Binding socket to the addres
    if (bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Can't bind the socket");
        exit(1);
    }

    socklen_t len = sizeof(client_addr);

    // Main loop
    while (true)
    {
        // Recieving data and checking it
        bytesFromClient = recvfrom(sd, buff, MAXLINE, MSG_WAITALL, (struct sockaddr *)&client_addr, &len);
        if (bytesFromClient == -1)
        {
            perror("Can't recieve data");
            exit(1);
        }

        // Checking if there is a empty datagram (1 - because of \n character)
        if (bytesFromClient == 1)
        {
            addEnding(errorMsg, isRN);

            // Informing client about error
            bytesToSent = sendError(sd, errorMsg, strlen(errorMsg), (struct sockaddr *)&client_addr, len);
            printf("\nUnable to read data from client\n");
            continue;
        }

        // Checking what kind of line ending the incoming message has (\r\n or \n)
        if (buff[bytesFromClient] == '\n' && buff[bytesFromClient - 1] == '\r')
        {
            isRN = true;
        }
        // Checking input if it's readable by server
        if (checkInput(buff, bytesFromClient))
        {
            printf("\nData from client: %s", buff);
        }
        else
        {
            addEnding(errorMsg, isRN);

            // If not readable informig client
            bytesToSent = sendError(sd, errorMsg, strlen(errorMsg), (struct sockaddr *)&client_addr, len);
            printf("\nUnable to read data from client\n");
            continue;
        }

        // Getting result
        result = performAction(buff, bytesFromClient - 1);
        if (result == -1)
        {
            // If data is too big server informs client that this data prowokes overflow
            printf("\nLarge data has come from client and overflow happend");
            addEnding(errorMsg, isRN);

            bytesToSent = sendError(sd, errorMsg, strlen(errorMsg), (struct sockaddr *)&client_addr, len);
            continue;
        }

        // Printing result
        printf("Result: %d\n", result);

        // Converting result from int to char*
        bytesToSent = sprintf(result_char, "%d", result);
        if (bytesToSent == -1)
        {
            perror("Can't convert data");
            addEnding(errorMsg, isRN);

            // Informing client that error occured
            bytesToSent = sendError(sd, errorMsg, strlen(errorMsg), (struct sockaddr *)&client_addr, len);
            continue;
        }

        // Prepearing answer
        strcat(answer, result_char);
        addEnding(answer, isRN);

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

        // Clearing buffors
        memset(answer, 0, 12);
        memset(buff, 0, MAXLINE);
        memset(result_char, 0, 10);
    }
}