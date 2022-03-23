#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <limits.h>
#include <stdint.h>

#define MAXLINE 1024

// Server desckryptor
int sd;

// Function to handle signal
void signal_handler(int signum);

// Function to create socket for server
int createSocket();

// Function to check if input is a number
bool isNumber(char input);

// Function to check if input has some different data except from numbers
bool checkInput(char *data, int lenght);

// Server actions
int performAction(char *data, int lenght);

// Function to create and start server
void startSumServer(int port);

// Function to add a proper ending to the line
void addEnding(char* data, bool type);

// Function to send error message to the client
int sendError(int sd, char* msg, size_t msg_len, struct sockaddr* clientAddr, socklen_t len);

#endif