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
#include <errno.h>

#define MAX_LINE 1024

// Function to handle signal
void signal_handler(int signum);

// Function to create socket for server
int createSocket();

// Function to check if input is a number
bool isNumber(char input);

// Function to check if input has some different data except from numbers
bool checkInput(char *data, int length, bool *isRN);

// Server actions
unsigned long long int performAction(char *data, int length);

// Function to create and start server
void startSumServer(int port);

// Function to add a proper ending to the line
void addEnding(char *data);

// Function to send error message to the client
int sendError(int sd, char *msg, size_t msg_len, struct sockaddr *clientAddr, socklen_t len);

// Function to prepare sockaddr
void prepare_sockaddr(struct sockaddr_in6 *addr, int port);

#endif