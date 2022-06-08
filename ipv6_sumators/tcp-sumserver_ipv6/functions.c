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
    int server_descriptor = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (server_descriptor == -1)
    {
        perror("Can't create socket");
        exit(1);
    }

    // Structure dor server data
    struct sockaddr_in6 address;

    // Filling sockaddr_in
    memset(&address, 0, sizeof(address));
    address.sin6_family = AF_INET6;
    address.sin6_port = htons(port);
    address.sin6_addr = in6addr_any;

    // Cast to const struct sockaddr for passing into functions
    const struct sockaddr *addr = (const struct sockaddr *)&address;

    // Binding socket
    if (bind(server_descriptor, addr, sizeof(address)) == -1)
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
        // Moving through the buffer
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

    // Length of answer
    int length = strlen(answer_ch);

    // Sending answer
    byteN = write(clientDescriptor, answer_ch, length);
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

    unsigned short int length = strlen(message);
    if (length <= 0)
    {
        return false;
    }

    // printf("[+] Checking data: %s| with length: %i\n", message, length);

    // Checking data
    for (int i = 0; i < length; i++)
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
            message[i] = ' ';
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
                printf("[-] Space in the beginning or in the end of message\n");
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

void showNewClient(int client_descriptor, char *client_address, int port)
{
    // Structure for holding informations about client
    struct sockaddr_in _addr;
    socklen_t length = sizeof(_addr);

    // Getting name of client
    if (getpeername(client_descriptor, (struct sockaddr *)&_addr, &length) == -1)
    {
        perror("Can't get name of peer");
        exit(1);
    }

    // Converting ip from binary to text and showing it's address
    const char *client_ip = inet_ntop(AF_INET, &(_addr.sin_addr), client_address, INET_ADDRSTRLEN);
    if (client_ip != NULL)
    {
        printf("-------------------------------\n");
        printf("Connected with: %s:%d|\ndescriptor number: %i\n\n", client_address, port, client_descriptor);
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