#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>
#include "functions.h"

// Socket descriptor
int sd;

int createSocket()
{
    // Socket creation with UDP specifications
    int result = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (result == -1)
    {
        perror("Can't create socket");
        exit(1);
    }
    return result;
}

// Function to close socket
void sig_handler(int signum)
{
    sleep(1);
    printf("\nClosing server...\n");
    if (close(sd) == -1)
    {
        perror("Can't close server");
        exit(1);
    }
    exit(0);
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

void startServer(int port)
{
    // Creating socket
    sd = createSocket();

    bool isActive = true;

    // Ilość opracowanych bajtów
    ssize_t byteN;

    // Structure dor server data
    struct sockaddr_in addres;

    // Accepted socket descriptor
    int clientDescriptor;

    // Addres of client
    char client_addres[INET_ADDRSTRLEN];

    // Number for sending
    unsigned long long int answer_int = 0;

    // Number for sending in char* type
    char answer_ch[12];

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

    printf("\nServer is created, and listening on this port: %d\n", port);

    // Main loop
    while (true)
    {
        memset(answer_ch, 0, sizeof(answer_ch));

        // Accepting connection
        clientDescriptor = accept(sd, NULL, 0);
        if (clientDescriptor == -1)
        {
            perror("Can't accept connection");
            exit(1);
        }

        // Strutrura dla przechowywania danych o połączonym urządzeniu
        struct sockaddr_in _addr;

        socklen_t lenght = sizeof(_addr);

        // Getting name of client
        if (getpeername(clientDescriptor, (struct sockaddr *)&_addr, &lenght) == -1)
        {
            perror("Can't get name of peer");
            exit(1);
        }

        // Converting ip from binary to text
        const char *client_ip = inet_ntop(AF_INET, &(_addr.sin_addr), client_addres, INET_ADDRSTRLEN);
        if (client_ip != NULL)
        {
            printf("------------------------------\n");
            printf("Connected with: %s:%d\n\n", client_addres, port);
        }

        if (!performAction(&answer_int, clientDescriptor))
        {
            sendError(clientDescriptor);
        }

        printf("Closing connection\n");
        if (close(clientDescriptor) == -1)
        {
            perror("Can't close the connection");
            exit(1);
        }
        printf("------------------------------\n");
    }
    if (close(sd) == -1)
    {
        perror("Can't close server");
        exit(1);
    }
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, sig_handler);

    // Port na którym gniazdko będzie słuchać i czekać na połączenie
    int port = 2020;

    if (argc == 2)
    {
        port = strtol(argv[1], NULL, 10);
        if (port == 0) // Sprawdzenie czy udała się konwersja na int
        {
            perror("Wrong input");
            exit(1);
        }
    }
    if (port < 1024) // Sprawdzenie czy podany port nie jest zamały
    {
        printf("\nGiven port is to small\n");
        exit(0);
    }

    startServer(port);

    return 0;
}