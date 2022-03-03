/*
    Powyższy program prawdopodobnie używał indeksowania tablic. 
    Jeśli tak, to przerób go aby korzystał ze wskaźników i aby nigdzie w kodzie nie występowało liczby[i].
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
            *(arr + i) = input; // Dodanie danych do tablicy
        }
    }

    for (int i = 0; i < SIZE; i++)               //
    {                                            //
        if (*(arr + i) < 100 && *(arr + i) > 10) //
        {                                        // Wypisanie danych
            printf("%d\n", *(arr + i));          //
        }                                        //
    }                                            //

    return 0;
}