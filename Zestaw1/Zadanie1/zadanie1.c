/*
    Napisz program w C deklarujący tablicę int liczby[50] i wczytujący do niej z klawiatury kolejne liczby.
    Wczytywanie należy przerwać gdy użytkownik wpisze zero lub po wczytaniu 50 liczb.

    Dodaj do powyższego programu pętlę for idącą przez wszystkie wczytane liczby i drukującą te z nich,
    które są większe od 10 i mniejsze od 100.
*/

#include <stdio.h>

int main(int argc, char const *argv[])
{
    // Rozmiar tablicy
    const int SIZE = 50;
    // Tablica liczb całkowitych
    int arr[SIZE];
    // zmienna dla przechowywania danych podanych przez użytkownika
    int input;

    printf("Write 50 numbers:\n");
    for (int i = 0; i < SIZE; i++)
    {
        scanf("%d", &input); // Odczyt danych
        if (input == 0)      // Jeśli 0 jest podane to wyjdzie z pętli wcześniej
        {
            break;
        }
        else
        {
            arr[i] = input; // Dodanie danych do tablicy
        }
    }

    printf("\nYou wrote:\n");
    for (int i = 0; i < SIZE; i++)       //
    {                                    //
        if (arr[i] < 100 && arr[i] > 10) //
        {                                //  Wypisanie danych
            printf("%d\n", arr[i]);      //
        }                                //
    }                                    //

    return 0;
}