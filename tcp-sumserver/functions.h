#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <limits.h>

// Function to check if the charactes represents number
bool isNumber(char input);
// Function to send an error to the client
void sendError(int asd);
// Function to add "\r\n" at the end of message
void addEnding(char *data);
// Function to create socket
int createSocket(int port);
// Function to close connection with client
int closeConnection(int con);
// Function to sum numbers that contained in message
unsigned long long int sumNumbers(char *data);
// Function to send answer from server to client
void sendData(unsigned long long int answer_int, int clientDescriptor);
// Function to communicate with client
bool performAction(unsigned long long int *number, int cd);
// Function to show addres of new client that is connected
void showNewClient(int cliend_descriptor, char* client_addres, int port);
// Set timeout
int setTimeout(int socket);

#endif // FUNCTIONS_H