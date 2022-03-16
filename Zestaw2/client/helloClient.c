/*
    Napisz prostego klienta, który łączy się (użyj socket i connect) z usługą
    wskazaną argumentami podanymi w linii komend (adres IPv4 w argv[1], numer portu TCP w argv[2]),
    drukuje na ekranie wizytówkę zwróconą przez serwer i kończy pracę.

    Pamiętaj o zasadzie ograniczonego zaufania i weryfikuj,
    czy odebrane bajty to na pewno drukowalne znaki ASCII zanim je prześlesz na stdout.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Funkcja dla tworzenia gniazdka
int createSocket()
{
    int result = socket(AF_INET, SOCK_STREAM, 0);
    if (result == -1)
    {
        perror("Can't craete socket");
        exit(1);
    }
    return result;
}

// Funkcja dla sprawdzania drukowalności danych
bool isDrowable(void *input, int size)
{
    char *buf = (char *)input;

    for (int i = 0; i < size; i++)
    {
        if (buf[i] < 32 || buf[i] > 126)
        {
            return false;
        }
    }
    return true;
}

// Funkcja dla sprawdzania drukowalności pojedynczego znaku
bool isDrowableChar(const char c)
{
    if (c < 32 || c > 126)
    {
        return false;
    }
    return true;
}

// Funkcja dla tworzenia i urachamiania klientu
void startClient(const char *ip, int port)
{
    // Główny buffor dla przechowywania danych
    char buff[1];

    // Ilość opracowanych bajtów
    ssize_t byteN;

    // Tworzenie struktury dla przechowywania danych do połączenia
    struct sockaddr_in adres;

    // tworzenie klienckiego despkryptora
    int csd = createSocket();

    // Wypełnianie sockaddr_in
    memset(&adres, 0, sizeof(adres));
    adres.sin_family = AF_INET;
    adres.sin_port = htons(port);
    adres.sin_addr.s_addr = inet_addr(ip);

    // Połączenie z serwerem
    int con = connect(csd, (struct sockaddr *)&adres, sizeof(adres));
    if (con == -1)
    {
        perror("Can't connect to the server");
        exit(1);
    }

    printf("\n--- Connected to a server: %s:%d ---\n\n", ip, port);

    printf("Starting transmission...\n\n");

    // Odczyt danych z serwera
    while ((byteN = read(csd, buff, sizeof(buff))) != 0)
    {
        if (byteN == -1)
        {
            perror("Can't read data from server");
            exit(1);
        }
        // Sprawdzanie drukowalności danych
        if(isDrowableChar(buff[0]))
        {
            printf("%c", buff[0]);
        }
    }

    printf("\nTransmission is over.\n");

    if (close(csd) == -1)
    {
        perror("Can't close socket");
        exit(1);
    }
}

int main(int argc, char const *argv[])
{

    if (argc < 3)
    {
        printf("You wrote incorrect amount of arguments\n");
        printf("\n*** Usage:\n first argument  - ip of the server\n");
        printf(" second argument - port of the server you want to create.\n\n");
        exit(0);
    }
    // Port na którym gniazdko będzie słuchać i czekać na połączenie
    int port = strtol(argv[2], NULL, 10);
    if (port == 0) // Sprawdzenie czy udała się konwersja na int
    {
        perror("Wrong input");
        exit(1);
    }

    startClient(argv[1], port);

    return 0;
}
