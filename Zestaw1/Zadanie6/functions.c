#include "functions.h"

int openFile(const char *fileName)
{
    int file = open(fileName, O_RDONLY); // otwieranie pliku z flagiem dla czytania
    if (file == -1)                      // jeśli nie uda się rzuca error
    {
        perror("Can't open the file to read");
        _exit(-1);
    }
    return file;
}

int openFileToWrite(const char *fileName, mode_t mode)
{
    int file = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, mode); // otwieranie lub tworzenie pliku z odpowiednimi flagami dla zapisu
    if (file == -1)                                                // jeśli nie uda się rzuca error
    {
        perror("Can't open the file to write");
        _exit(-1);
    }
    return file;
}

int closeFile(int fid)
{
    int result = close(fid); // Zamykanie deskryptora pliku
    if (result == -1)        // jeśli nie uda się rzuca error
    {
        perror("Can't close the file");
        _exit(-1);
    }
    return result;
}

int readFile(int fildes, char *data)
{
    int numberOfBytes = 0;       // Ilość odczytanych bajtów
    char buff[1];                // buffor w ktorym znajduję się aktualna cząstka danych
    size_t nbyte = sizeof(buff); // Zadanie ilości bajtów dla czytania
    int bufferSize = 0;          // Zmienna dla sprawdzania stanu funkcji

    printf("\nReading from a file...\n");

    while ((bufferSize = read(fildes, buff, nbyte)) != 0) // Odczyt danych z pliku
    {
        if (bufferSize == -1) // Jeśli nie potrafi odczytać plik rzuca error
        {
            perror("Can't read the file");
            _exit(-1);
        }
        numberOfBytes += bufferSize; // Inkrementacja ilości odczytanych bajtów
        data = strcat(data, buff);   // Konkatenacja danych dla zapisania w ogólny buffor
    }
    printf("Reading is complete.\n");
    return numberOfBytes;
}

void writeToFile(int fildes, char *data, int lenght)
{
    int bufferSize = 0; // Zmienna dla sprawdzania stanu funkcji

    printf("\nWriting to a file...\n");

    bufferSize = write(fildes, data, lenght + 1); // Zapisanie daych do pliku
    if (bufferSize == -1)                         // Jeśli nie potrafi odczytać plik rzuca pomyłkę
    {
        perror("Can't write the file");
        _exit(-1);
    }

    printf("Writing complete.\n");
}

void writeOut(char *data)
{
    char input; // Zmienna pokazująca wybór użytkownika

    printf("\n Do you want to display data? [Y/n]\n");

    while (scanf("%c", &input)) // Odczyt wyboru użytkownika
    {
        if (input == 'y' || input == 'Y')
        {
            printf("%s", data); // Wypisanie danych na stdout
            break;
        }
        else if (input == 'n' || input == 'N')
        {
            break;
        }
        else
        {
            printf("\ninput error, Do you want to display data? [Y/n]\n");
        }
    }
}