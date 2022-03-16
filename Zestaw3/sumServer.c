/*
    Komunikacja pomiędzy klientem a serwerem odbywa się przy pomocy datagramów.
    Klient wysyła datagram zawierający liczby,
    serwer odpowiada datagramem zawierającym pojedynczą liczbę (obliczoną sumę) bądź komunikat o błędzie.

    Zawartość datagramów interpretujemy jako tekst w ASCII.
    Ciągi cyfr ASCII interpretujemy jako liczby dziesiętne. Datagram może zawierać albo cyfry i spacje,
    albo pięć znaków składających się na słowo „ERROR”; żadne inne znaki nie są dozwolone (ale patrz następny akapit).

    Aby ułatwić ręczne testowanie serwera przy pomocy ncat,
    serwer może również akceptować datagramy mające na końcu dodatkowy znak \n (czyli bajt o wartości 13)
    albo dwa znaki \r\n (bajty 10, 13). Serwer może wtedy, ale nie musi, dodać \r\n do zwracanej odpowiedzi.
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
#include <limits.h>
#include <stdint.h>

#define MAXLINE = 1024

// Server desckryptor
int sd;

// Function to handle signal
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

bool performAction(char *data, int lenght)
{
    int numbers[lenght / 2];

    for (int i = 0; i < lenght; i++)
    {
        if (data[i] == ' ')
        {
            continue;
        }
        
    }
    
}

// Function to create and start server
void startSumServer(int port)
{
}

int main(int argc, char const *argv[])
{

    return 0;
}
