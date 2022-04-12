#include "functions.h"

bool isNumber(char input)
{
    // Checking if a character is a number
    if (input > 57 || input < 48)
    {
        return false;
    }
    return true;
}

void sendError(int asd)
{
    const char error_msg[8] = "ERROR\r\n";
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
    // Socket creation with UDP specifications
    int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == -1)
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
    if (bind(sd, addr, sizeof(addres)) == -1)
    {
        perror("Can't bind socket");
        exit(1);
    }

    // Making socket to listen
    if (listen(sd, 10) == -1)
    {
        perror("Can't make the socket to listen");
        exit(1);
    }

    return sd;
}

unsigned long long int sumNumbers(char *data)
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

void sendData(unsigned long long int answer_int, int clientDescriptor)
{
    ssize_t byteN;
    char answer_ch[12];
    byteN = sprintf(answer_ch, "%lld", answer_int);
    if (byteN == -1)
    {
        perror("Can't convert data");
        sendError(clientDescriptor);
        return;
    }
    addEnding(answer_ch);
    printf("Result: %s", answer_ch);

    // Sending answer
    byteN = write(clientDescriptor, answer_ch, sizeof(answer_ch));
    if (byteN == -1)
    {
        perror("Can't send data");
    }
    printf("Answer was sent successfully\n============================\n");
}

bool performAction(unsigned long long int *number, int cd)
{
    signed short int space_counter = 0;
    char input[2] = {0, 0};
    char message[1024];
    ssize_t byteN;

    memset(message, 0, sizeof(message));

    while (byteN = read(cd, input, sizeof(input) - 1) != 0)
    {
        if (byteN == -1)
        {
            perror("Can't read data");
            return false;
        }
        if (isNumber(input[0]))
        {
            space_counter = 0;
            strncat(message, input, 1);
        }
        if (input[0] == ' ')
        {
            space_counter++;
            if (space_counter > 1)
            {
                return false;
            }
            strcat(message, input);
        }
        if (input[0] == '\r')
        {
            continue;
        }
        if (input[0] == '\n')
        {
            printf("Message form the server: %s\n", message);
            space_counter = 0;
            *number = sumNumbers(message);

            memset(input, 0, sizeof(input));
            memset(message, 0, sizeof(message));

            if (*number == -1 || *number == 0)
            {
                return false;
            }
            sendData(*number, cd);
        }
        memset(input, 0, sizeof(input));
    }
    return true;
}