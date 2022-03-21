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

        if(number > INT_MAX - result)
        {
            return -1;
        }
        result += number;
    }
    return result;
}

void startSumServer(int port)
{
    // Creating socket
    sd = createSocket();

    // Main buffer
    char buff[MAXLINE];

    // Buffer for the server answer
    char answer[MAXLINE / 4];

    // State that shows if client system requiers to has \r\n in the end of message
    bool isRN = false;

    int bytesFromClient, bytesToSent, result;

    // result string
    char result_char[4];

    // Error messages
    char *errorMsg = "\nIncorect data...\n";
    char *overflowMsg = "\nOverflow...\n";

    // Structures for server and client
    struct sockaddr_in server_addr, client_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Compleating server structure
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

        if (checkInput(buff, bytesFromClient))
        {
            printf("\nMessage from client: %s\n", buff);
        }
        else
        {
            bytesToSent =  sendto(sd, errorMsg, strlen(errorMsg), MSG_CONFIRM, (struct sockaddr *)&client_addr, len);
            if(bytesToSent == -1)
            {
                perror("Can't send an error");
                exit(1);
            }
            printf("\nUnable to read data from client\n");
            continue;
        }

        // Preapearing the answer from server
        result = performAction(buff, bytesFromClient - 1);
        if(result == -1)
        {
            printf("\nLarge data has come from client and overflow happend");
            bytesToSent = sendto(sd, overflowMsg, strlen(overflowMsg), MSG_CONFIRM, (struct sockaddr *)&client_addr, len);
            if(bytesToSent == -1)
            {
                perror("Can't send an error");
                exit(1);
            }
            continue;
        }

        printf("\n%d\n", result);
        strcat(answer, "Answer: ");
        sprintf(result_char, "\n%d\n", result);
        strcat(answer, result_char);
        if (buff[bytesFromClient] == '\n' && buff[bytesFromClient - 1] == '\r')
        {
            strcat(answer, "\r\n");
        }
        else
        {
            strcat(answer, "\n");
        }

        // Sending answer
        bytesToSent = sendto(sd, answer, strlen(answer), MSG_CONFIRM, (struct sockaddr *)&client_addr, len);
        if (bytesToSent == -1)
        {
            perror("Can't send a message");
        }
        {
            printf("\n - Answer was sent successfully...\n");
        }

        // Clearing buffors
        memset(answer, 0,MAXLINE/4);
        memset(buff, 0, MAXLINE);
    }
}