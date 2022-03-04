/*
    Opracuj funkcję testującą czy przekazany jej bufor zawiera tylko i wyłącznie drukowalne znaki ASCII,
    tzn. bajty o wartościach z przedziału domkniętego [32, 126].
    Funkcja ma mieć następującą sygnaturę: bool drukowalne(const void * buf, int len).

    Pamiętaj o włączeniu nagłówka <stdbool.h>, bez niego kompilator nie rozpozna ani nazwy typu bool,
    ani nazw stałych true i false.

    Zaimplementuj dwa warianty tej funkcji,
    w pierwszym wariancie funkcja ma pobierać kolejne bajty z bufora przy pomocy operacji indeksowania tablic,
    w drugim ma używać wskaźnika przesuwającego się z bajtu na bajt.
*/

#include <stdio.h>
#include <stdbool.h>

// Funckja dla sprawdzania czy podany buffor zawiera drukowalne znaki ASCII (oparta na indeksowaniu tablicy)
bool drukowalne(const void *buf, int len)
{
    const char *char_buf = (char *)buf; // Konwersja z "ogólnego" wskażnika do wskażnika na char

    for (int i = 0; i < len; i++) // Sprawdzenie danych
    {
        if (char_buf[i] <= 32 || char_buf[i] >= 126)
        {
            return false;
        }
    }
    return true;
}

// Funckja dla sprawdzania czy podany buffor zawiera drukowalne znaki ASCII (oparta na przesuwającym się wskażniku)
bool drukowalneP(const void *buf, int len)
{
    const char *char_buf = (char *)buf; // Konwersja z "ogólnego" wskażnika do wskażnika na char

    for (int i = 0; i < len; i++) // Sprawdzenie danych
    {
        if (*(char_buf + i) <= 32 || *(char_buf + i) >= 126)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char const *argv[])
{
    // Rozmiar dla tablic
    const int SIZE = 10;
    // Pierwsza tablica, elementy której są w przedziale
    char arr1[SIZE];
    // Druga tablica, elementy której nie są w przedziale
    char arr2[SIZE];

    // Zapełnienie tablic
    for (int i = 0; i < SIZE; i++)
    {
        arr1[i] = (char)(33 + i);
        arr2[i] = (char)(22 + i);
    }
    // Wypisanie elementów
    for (int i = 0; i < SIZE; i++)
    {
        printf("1 - %c ", arr1[i]);
        printf("2 - %c\n", arr2[i]);
    }

    // Testowanie funkcji na podstawie indeksowania tablicy
    if (drukowalne(arr1, SIZE))
    {
        printf("\nAll elements of first array are drowable.\n");
    }
    else
    {
        printf("\nAll elements of first array are not drowable.\n");
    }
    // Testowanie funkcji na podstawie przesuwania wskażnika
    if (drukowalneP(arr2, SIZE))
    {
        printf("\nAll elements of second array are drowable.\n");
    }
    else
    {
        printf("\nAll elements of second array are not drowable.\n");
    }

    return 0;
}
