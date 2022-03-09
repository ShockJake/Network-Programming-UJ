/*
    Napisz prosty serwer zwracający wizytówkę.
    Powinien tworzyć gniazdko TCP nasłuchujące na porcie o numerze podanym jako argv[1] (użyj socket, bind i listen),
    następnie w pętli czekać na przychodzące połączenia (accept), wysyłać ciąg bajtów Hello, world!\r\n jako swoją wizytówkę,
    zamykać odebrane połączenie i wracać na początek pętli.

    Pętla ma działać w nieskończoność, aby przerwać działanie programu trzeba będzie użyć Ctrl-C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Funckja dla wypełnienia struktury z danymi dla połączenia
void fillSocketFields(struct sockaddr_in *adres, int port)
{
    memset(&adres, 0, sizeof(adres));
    adres->sin_family = AF_INET;
    adres->sin_port = htons(port);
    adres->sin_addr.s_addr = htonl(INADDR_ANY);
}

// Funkcja tworząca gniazdo
int createSocket()
{
    int result = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (result == -1) // Sprawdzaine czy udało się
    {
        perror("Can't create a socket");
        _exit(-1);
    }
    return result;
}

// Funkcja do tworzenia i startowania serweru
void startHelloServer(int port)
{
    // Wyzytówka do wypisania
    const char *msg = "Hello World!\r\n";

    // Ilość opracowanych bajtów
    ssize_t byteN;

    // Tworzenie struktury dla przechowywania danych do połączenia
    struct sockaddr_in adres;

    // Tworzenie gniazdka
    int sfd = createSocket();

    // Wypełnienie sockaddr_in
    // fillSocketFields(&adres, port);
    memset(&adres, 0, sizeof(adres));
    adres.sin_family = AF_INET;
    adres.sin_port = htons(port);
    adres.sin_addr.s_addr = htonl(INADDR_ANY);

    // Cast do const struct sockaddr dla podania do funkcji
    const struct sockaddr *addr = (const struct sockaddr *)&adres;

    // Bindowanie gniazdka
    if (bind(sfd, addr, sizeof(adres)) == -1)
    {
        perror("Can't bind socket");
        _exit(-1);
    }

    // Ustawienie gniazdka na słuchanie
    if (listen(sfd, 0) == -1)
    {
        perror("Can't make the socket to listen");
        _exit(-1);
    }

    printf("\nServer is created, and listening on this port: %d\n", port);

    // Główna pętla
    while (true)
    {
        // Akceptujemy deskryptor połączenia
        int acd = accept(sfd, NULL, 0);
        if (acd == -1)
        {
            perror("Can't accept connection");
            _exit(-1);
        }

        struct sockaddr_storage _addr;
        socklen_t lenght = sizeof(_addr);

        if (getsockname(acd, (struct sockaddr *)&_addr, &lenght) == -1)
        {
            perror("Can't get name of peer");
            _exit(-1);
        }

        printf("Connected with: %s : %d\n", _addr.__ss_padding, port);

        // Wysłanie danych do klientu
        byteN = write(acd, msg, 15);
        if (byteN == -1)
        {
            perror("Can't send data");
            close(acd);
            continue;
        }

        printf("Message has been sent succsefully\n");

        // Zamykanie połączenia
        if (close(acd) == -1)
        {
            perror("Can't close the connection");
            _exit(-1);
        }
    }
    if (close(sfd) == -1)
    {
        perror("Can't close the socket");
        _exit(-1);
    }
}

int main(int argc, char const *argv[])
{

    // Sprawdzanie ilości argumentów
    if (argc < 2)
    {
        printf("You wrote incorrect amount of arguments\n");
        printf("\n*** Usage:\n first argument - port of the server you want to create.\n\n");
        _exit(0);
    }
    // Port na którym gniazdko będzie słuchać i czekać na połączenie
    int port = strtol(argv[1], NULL, 10);
    if (port == 0) // Sprawdzenie czy udała się konwersja na int
    {
        perror("Wrong input (port)");
        _exit(-1);
    }
    if (port < 1024) // Sprawdzenie czy podany port nie jest zamały
    {
        printf("\nGiven port is to small\n");
        _exit(0);
    }
    startHelloServer(port);

    return 0;
}
