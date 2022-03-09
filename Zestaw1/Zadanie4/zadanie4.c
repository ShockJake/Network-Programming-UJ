/*
    A teraz opracuj wersję, która jako argument dostaje łańcuch w sensie języka C,
    czyli ciąg niezerowych bajtów zakończony bajtem równym zero (ten końcowy bajt nie jest uznawany za należący do łańcucha).
    Ta wersja funkcji powinna mieć taką sygnaturę: bool drukowalne(const char * buf).

    Tu też zaimplementuj dwa warianty: używający indeksowania i używający przesuwającego się wskaźnika.
*/

#include <stdio.h>
#include <stdbool.h>

// Funckja dla sprawdzania czy podany buffor zawiera drukowalne znaki ASCII (oparta na indeksowaniu tablicy)
bool drukowalne(const char *buf)
{
    const int SIZE = sizeof(buf) - sizeof(buf[0]); // Wyliczanie rozmiaru

    for (int i = 0; i < SIZE; i++) // Sprawdzanie danych
    {
        if (buf[i] < 32 || buf[i] > 126)
        {
            return false;
        }
    }
    return true;
}

// Funckja dla sprawdzania czy podany buffor zawiera drukowalne znaki ASCII (oparta na przesuwającym się wskażniku)
bool drukowalneP(const char *buf)
{
    const int SIZE = sizeof(buf) - sizeof(buf[0]); // Wyliczanie rozmiaru

    for (int i = 0; i < SIZE; i++) // Sprawdzanie danych
    {
        if (*(buf++) < 32 || *(buf++) > 126)
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

    // Testowanie funkcji
    if (drukowalne(arr1))
    {
        printf("\nAll elements of first array are drowable.\n");
    }
    else
    {
        printf("\nAll elements of first array are not drowable.\n");
    }

    if (drukowalneP(arr2))
    {
        printf("\nAll elements of second array are drowable.\n");
    }
    else
    {
        printf("\nAll elements of second array are not drowable.\n");
    }

    return 0;
}
