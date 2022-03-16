/*
    Spróbuj napisać parę klient-serwer komunikującą się przy pomocy protokołu UDP.
    Pamiętaj o tym, że UDP nie nawiązuje połączeń,
    więc to klient będzie musiał jako pierwszy wysłać jakiś datagram, a serwer na niego odpowie.

    Sprawdź, czy możliwe jest wysyłanie pustych datagramów (tzn. o długości zero bajtów) —
    jeśli tak, to może niech klient właśnie taki wysyła?
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/signal.h>

#define MAXLINE 1024

// socket deskrytor
int sd;

// Funkcja dla tworzenia deskryptora
int createSocket()
{
    int result = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (result == -1)
    {
        perror("Can't create a socket");
        exit(1);
    }
    return result;
}

// Funkcja dla przechwytu sygnału
void signal_handler(int signum)
{
    printf("\nClosing server...\n");
    if (close(sd) == -1)
    {
        perror("Can't close the server");
	    exit(1);
    }
    exit(0);
}

// Funkcja dla potwierdzenia połączenia
bool verifyConnection(int sockD, char buf[], socklen_t lenght, struct sockaddr *sockaddr_in)
{
    int n;
    n = recvfrom(sockD, buf, MAXLINE, MSG_WAITALL, sockaddr_in, &lenght);
    if (n > 0)
    {
        printf("\n-!!!- Connection is established successfully...\n");
        printf("\nMessage from client: %s", buf);
        return true;
    }
    return false;
}

void startUDPServer(int port)
{
    // Tworzenie deskryptora dla soketu
    sd = createSocket();

    // Główny buffor
    char buf[MAXLINE];

    // Powiadomienie dla klientu
    char msg[13] = "Hello World\n";

    // Struktury dla strony serweru i klientu
    struct sockaddr_in server_addr, client_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Wypełnianie informacji o serwerze
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Can't bind the socket");
        exit(1);
    }

    socklen_t len = sizeof(client_addr);

    // Główna pętla
    while (true)
    {
        if (verifyConnection(sd, buf, len, (struct sockaddr *)&client_addr))
        {
            printf("Sending a message...\n");

            // Wysyłka powiadomienia
            int result = sendto(sd, msg, strlen(msg), MSG_CONFIRM, (struct sockaddr *)&client_addr, len);
            if (result == -1)
            {
                perror("Can't send a message");
                printf("\nCan't send a message\n");
            }
            else
            {
                printf("\nMessage was sent successfully...\n");
            }
        }
    }
}

int main(int argc, char const *argv[])
{

    signal(SIGINT, signal_handler);

    // Sprawdzanie ilości argumentów
    if (argc < 2)
    {
        printf("You wrote incorrect amount of arguments\n");
        printf("\n*** Usage:\n first argument - port of the server you want to create.\n\n");
        exit(0);
    }
    // Port na którym gniazdko będzie słuchać i czekać na połączenie
    int port = strtol(argv[1], NULL, 10);
    if (port == 0) // Sprawdzenie czy udała się konwersja na int
    {
        perror("Wrong input");
        exit(1);
    }
    if (port < 1024) // Sprawdzenie czy podany port nie jest zamały
    {
        printf("\nGiven port is to small\n");
        exit(0);
    }

    printf("\nStarting UDP server\n");
    startUDPServer(port);

    return 0;
}
