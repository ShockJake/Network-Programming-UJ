/*
    Zaimplementuj program kopiujący dane z pliku do pliku przy pomocy powyższych funkcji.
    Zakładamy, że nazwy plików są podawane przez użytkownika jako argumenty programu (tzn. będą dostępne w tablicy argv).
    Zwróć szczególną uwagę na obsługę błędów — każde wywołanie funkcji we-wy musi być opatrzone testem sprawdzającym,
    czy zakończyło się ono sukcesem, czy porażką.

    Funkcje POSIX zwracają -1 aby zasygnalizować wystąpienie błędu,
    i dodatkowo zapisują w globalnej zmiennej errno kod wskazujący przyczynę wystąpienia błędu (na dysku nie ma pliku o takiej nazwie,
    brak wystarczających praw dostępu, itd.).
    Polecam Państwa uwadze pomocniczą funkcję perror, która potrafi przetłumaczyć ten kod na zrozumiały dla człowieka komunikat i wypisać go na ekranie.
*/

#include "functions.h"
#include <stdlib.h>
#define MAX_SIZE 512 // Maksymalny rozmiar buffora

int main(int argc, char const *argv[])
{
    printf("\nProgram to write data from a file to another file\n");
    printf("\n\nFile name %s\n", argv[1]);

    // kontener dla danych
    char data[MAX_SIZE];
    // deskryptor dla pliku do odczytania
    int fileToRead;
    // deskryptor dla pliku do wpisywania
    int fileToWrite;
    // flagi dla utworzonego pliku
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    // imie pliku do odczytania
    const char *readFileName = argv[1];
    // imie pliku do wpisywania
    char *writeFileName = "/home/stas/Projects/NetProg/Zestaw1/Zadanie6/new_file.txt";

    // otwieranie deskryptorów dla plików
    fileToRead = openFile(readFileName);
    fileToWrite = openFileToWrite(writeFileName, mode);

    // odczyt
    int lenght = readFile(fileToRead, data);
    // wypisywanie na stdout
    writeOut(data);
    // wpisywanie w pliks
    writeToFile(fileToWrite, data, lenght);

    // zamknięcie
    closeFile(fileToRead);
    closeFile(fileToWrite);

    return 0;
}
