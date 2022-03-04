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

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    printf("\nProgram to write data from a file to another file\n");
    printf("\n\nFile name: %s\n", argv[1]);

    // deskryptor dla pliku do odczytu
    int fileToRead;
    // deskryptor dla pliku do wpisania
    int fileToWrite;
    // główny buffor dla przechowywania danych
    char buff[1];
    // ilość bajtów dla oddziaływania
    size_t nbytes = sizeof(buff);
    // zmienna dla sprawdzania stanu funkcji
    ssize_t bufferSize;
    // zbiór flagów dla tworzenia pliku
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    // nazwa pliku do odczytu
    const char *readFileName = argv[1];
    // ścieżka oraz nazwa pliku dla wpisania danych
    char *writeFileName = "/home/stas/Projects/NetProg/Zestaw1/Zadanie7/new_file.txt";
    // Zmienna dla ślezenia ilości bajtów -pomocnicza-
    int i = 0;

    // Otwieranie deskryptora dla pliku do odczytu
    fileToRead = open(readFileName, O_RDONLY);
    if (fileToRead == -1)
    {
        perror("Can't open file to read");
        _exit(-1);
    }

    // Otwieranie deskryptora dla pliku do wpisania danych
    fileToWrite = open(writeFileName, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (fileToWrite == -1)
    {
        perror("Can't open / create file to write");
        _exit(-1);
    }

    printf("\nProcessing...\n");

    // Odczyt danych bajt po bajcie
    while ((bufferSize = read(fileToRead, buff, nbytes)) != 0)
    {
        if (bufferSize == -1)
        {
            perror("Unable to read the file");
            _exit(-1);
        }
        bufferSize = write(fileToWrite, buff, nbytes); // Wpysanie danych
        if (bufferSize == -1)
        {
            perror("Unable to write data to a file");
            _exit(-1);
        }
        if ((i % 25) == 0)  //
        {                   //
            printf(".\n");  // Powolnienie programu 
            sleep(1);       //
        }                   //
        i++;
    }
    printf("Done\n");
    return 0;
}
