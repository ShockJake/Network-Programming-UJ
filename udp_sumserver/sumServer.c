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

#include "functions.h"

void print_usage(int argc, char const *argv[])
{
    printf("\n*** Usage:\n first argument - port of the server you want to create (2020 - used by default).\n\n");
    printf("You provided:\n");
    for (int i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    exit(1);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, signal_handler);

    // Sprawdzanie ilości argumentów
    if (argc < 1)
    {
        print_usage(argc, argv);
    }

    // Port na którym gniazdko będzie słuchać i czekać na połączenie
    int port = 2020;

    if (argc > 1)
    {
        port = strtol(argv[1], NULL, 10);
    }
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
    start_sum_server(port);

    return 0;
}
