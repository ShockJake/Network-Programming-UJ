#include "functions.h"

bool isNumber(char input, unsigned short int *space_counter)
{
    if (input == ' ')
    {
        // printf("[+] Detected space = %i\n", *space_counter);
        *space_counter += 1;
        return true;
    }

    if (input == '\r' || input == '\n')
    {
        return true;
    }

    // Checking if a character is a number
    if (input < 58 && input > 47)
    {
        // printf("[+] Detected number %c(%i), spaces = %i\n", input, (int)input, *space_counter);
        *space_counter = 0;
        return true;
    }
    return false;
}

void sendError(int asd)
{
    const char error_msg[7] = "ERROR\r\n";
    // Sending error to the client
    ssize_t byteN = write(asd, error_msg, sizeof(error_msg));
    if (byteN == -1)
    {
        perror("Can't send error");
        exit(1);
    }
}

void addEnding(char *data)
{
    strcat(data, "\r\n");
}

int createSocket(int port)
{
    // Socket creation with TCP specifications
    int server_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_descriptor == -1)
    {
        perror("Can't create socket");
        exit(1);
    }

    // Structure dor server data
    struct sockaddr_in addres;

    // Filling sockaddr_in
    memset(&addres, 0, sizeof(addres));
    addres.sin_family = AF_INET;
    addres.sin_port = htons(port);
    addres.sin_addr.s_addr = htonl(INADDR_ANY);

    // Cast to const struct sockaddr for passing into functions
    const struct sockaddr *addr = (const struct sockaddr *)&addres;

    // Bining socket
    if (bind(server_descriptor, addr, sizeof(addres)) == -1)
    {
        if (close(server_descriptor) == -1)
        {
            perror("Can't close server");
            exit(1);
        }
        perror("Can't bind socket");
        exit(1);
    }

    // Making socket to listen
    if (listen(server_descriptor, 10) == -1)
    {
        if (close(server_descriptor) == -1)
        {
            perror("Can't close server");
            exit(1);
        }
        perror("Can't make the socket to listen");
        exit(1);
    }

    return server_descriptor;
}

unsigned long long int sumNumbers(char *data)
{
    unsigned long long int number = 0;
    unsigned long long int result = 0;

    // Separating buffer to tokens
    char *numberStr = strtok(data, " ");
    while (numberStr != NULL)
    {
        // Converting char* -> unsigned long long int
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
        // Moving througth the buffer
        numberStr = strtok(NULL, " ");
    }
    return result;
}

void sendData(unsigned long long int answer_int, int clientDescriptor)
{
    ssize_t byteN;
    char answer_ch[12];
    // Converting char* -> unsigned long long int
    byteN = sprintf(answer_ch, "%lld", answer_int);
    if (byteN == -1)
    {
        perror("Can't convert data");
        sendError(clientDescriptor);
        return;
    }
    addEnding(answer_ch);
    printf("Result: %s", answer_ch);

    // Lenght of answer
    int lenght = strlen(answer_ch);

    // Sending answer
    byteN = write(clientDescriptor, answer_ch, lenght);
    if (byteN == -1)
    {
        perror("Can't send data");
    }
    printf("Answer was sent successfully\n============================\n");
}

bool checkData(char *message)
{
    unsigned short int space_counter = 0;

    // Variable to check if the message has \r in it
    bool hasR = false;

    unsigned short int lenght = strlen(message);
    if (lenght <= 0)
    {
        return false;
    }

    // printf("[+] Checking data: %s| with lenght: %i\n", message, lenght);

    // Checking data
    for (int i = 0; i < lenght; i++)
    {
        if (isNumber(message[i], &space_counter))
        {
            if (space_counter >= 2)
            {
                printf("[-] To many spaces: %i\n", space_counter);
                return false;
            }
        }
        if (!isNumber(message[i], &space_counter))
        {
            printf("[-] Wrong data: %i : %i\n", (int)message[i], i);
            return false;
        }
        if (message[i] == '\n' && space_counter > 0)
        {
            printf("[-n] To many spaces: %i\n", space_counter);
            return false;
        }
        if (message[i] == '\r')
        {
            hasR = true;
            continue;
        }
        if (message[i] == '\n' && !hasR)
        {
            printf("[-] Lack of \\r character\n");
            return false;
        }
        if (message[i] == '\n')
        {
            if (message[0] == ' ' || message[i - 2] == ' ')
            {
                printf("[-] Space in the begining or in the end of message\n");
                return false;
            }
        }
    }
    return true;
}

bool performAction(unsigned long long int *number, int cd)
{
    // unsigned short int space_counter = 0;
    char input[2] = {0, 0};
    char message[1024];
    ssize_t byteN;
    memset(message, 0, sizeof(message));

    // Reading data from client
    while ((byteN = read(cd, input, sizeof(input) - 1)) != 0)
    {
        if (byteN == -1)
        {
            perror("Can't read data");
            return false;
        }
        strncat(message, input, 1);
        if (input[0] == '\n')
        {
            printf("============================\n");
            printf("Message form the client - %i:\n:: %s\n", cd, message);
            if (!checkData(message))
            {
                printf("Sending error...\n");
                // Sending errors
                sendError(cd);
            }
            else
            {
                // summing numbers
                *number = sumNumbers(message);

                // Sending data to the client
                sendData(*number, cd);
            }
            memset(input, 0, sizeof(input));
            memset(message, 0, sizeof(input));
        }
    }
    return true;
}

int closeConnection(int con)
{
    int num = close(con);
    if (num == -1)
    {
        perror("Can't close the connection");
    }
    return num;
}

void showNewClient(int client_descriptor, char *client_addres, int port)
{
    // Structure for holding informations about client
    struct sockaddr_in _addr;
    socklen_t lenght = sizeof(_addr);

    // Getting name of client
    if (getpeername(client_descriptor, (struct sockaddr *)&_addr, &lenght) == -1)
    {
        perror("Can't get name of peer");
        exit(1);
    }

    // Converting ip from binary to text and showing it's addres
    const char *client_ip = inet_ntop(AF_INET, &(_addr.sin_addr), client_addres, INET_ADDRSTRLEN);
    if (client_ip != NULL)
    {
        printf("-------------------------------\n");
        printf("Connected with: %s:%d|\ndescriptor number: %i\n\n", client_addres, port, client_descriptor);
    }
}

int setTimeout(int socket)
{
    // Structure for timeout specification
    struct timeval timeout;
    timeout.tv_sec = 20;
    timeout.tv_usec = 0.0;

    // Setting timeout for the client
    int result = setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (result == -1)
    {
        perror("Can't set timeout");
        return -1;
    }
    return result;
}