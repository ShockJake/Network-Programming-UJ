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

#define MAX_LINE 1024

// Funkcja dla tworzenia soketu
int createSocket()
{
    int result = socket(AF_INET, SOCK_DGRAM, 0);
    if (result == -1)
    {
        perror("Can't create a socket");
        _exit(-1);
    }
    return result;
}

void startUDPClient(const char *ip, int port)
{
    // Tworzenie deskryprora klienckiego (client deskryptor)
    int cd = createSocket();

    // Główny buffor
    char buf[MAX_LINE];

    // Powiadomienie dla wysyłania do serwera
    char *msg = "Hi, I like chocolate with caramel\n";

    struct sockaddr_in server_addr;

    // Zapełnienie danych o serwerze
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    int n, len;

    // Wysyłka powiadomienia do serwera
    sendto(cd, msg, strlen(msg), MSG_CONFIRM, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("\n-----\nMessage was successfully sent\n-----\n");

    // Przyjmowanie odwrotnego powiadomienia
    n = recvfrom(cd, buf, MAX_LINE, MSG_WAITALL, (struct sockaddr *)&server_addr, &len);
    if(n == -1)
    {
        printf("- !!! - Can't receive data from server\n");
    }
    buf[n] = '\0';
    printf("Server: %s", buf);

    close(cd);
}

int main(int argc, char const *argv[])
{
    // Sprawdzanie agrumentów
    if (argc < 3)
    {
        printf("You wrote incorrect amount of arguments\n");
        printf("\n*** Usage:\n first argument  - ip of the server\n");
        printf(" second argument - port of the server you want to create.\n\n");
        _exit(0);
    }
    // Port na którym gniazdko będzie słuchać i czekać na połączenie
    int port = strtol(argv[2], NULL, 10);
    if (port == 0) // Sprawdzenie czy udała się konwersja na int
    {
        perror("Wrong input");
        _exit(-1);
    }

    printf("\nStarting client:\n");
    startUDPClient(argv[1], port);
    printf("\nEnd of transmission...\n");

    return 0;
}
