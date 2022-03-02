#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// Function to open descrytor of a file
int openFile(const char *fileName);
// Function to create/recreate the file
int openFileToWrite(const char *fileName, mode_t mode);
// Fonction to close descryptor of a file
int closeFile(int fid);
// Function to read data from a file
int readFile(int fildes, char *buff);
// Function to write data to a file
void writeToFile(int fildes, char *buff, int lenght);
// Function to write data to a stdout
void writeOut(char *buff);

#endif // FUNCTIONS_H