#include "functions.h"

// Socket descriptor
int sd;

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

void startServer(int port)
{
    // Creating socket
    sd = createSocket(port);

    bool isActive = true;

    // Ilość opracowanych bajtów
    ssize_t byteN;

    // Accepted socket descriptor
    int clientDescriptor;

    // Addres of client
    char client_addres[INET_ADDRSTRLEN];

    // Number for sending
    unsigned long long int answer_int = 0;

    // Number for sending in char* type
    char answer_ch[12];

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