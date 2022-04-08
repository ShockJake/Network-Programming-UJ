#include "functions.h"

bool isNumber(char input)
{
    // Checking if a character is a number
    if (input > 57 || input < 48)
    {
        return false;
    }
    return true;
}

void sendError(int asd)
{
    const char error_msg[8] = "ERROR\r\n";
    ssize_t byteN = write(asd, error_msg, sizeof(error_msg));
    if (byteN == -1)
    {
        perror("Can't send error");
        exit(1);
    }
}

void addEnding(char *data)
{
    strcat(data, "\r\n");
}