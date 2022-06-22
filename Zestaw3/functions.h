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

// Server descriptor
int sd;

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
void addEnding(char* data);

// Function to send error message to the client
int sendError(int sd, char* msg, size_t msg_len, struct sockaddr* clientAddr, socklen_t len);

// Function to fill socket address structures
void fill_sock_addr_struct(struct sockaddr_in *server_addr, struct sockaddr_in *client_addr, int port);

// Function to bind socket
void bind_server_socket(int sd, struct sockaddr* server_addr);

// Function to clear buffers and variables
void clear_buffers_and_variables(char *answer, char *buff, char *result_char, bool *isRN); 

// Function to receive info
int receive_data(int sd, char *buff, struct sockaddr *client_addr, socklen_t *len);


#endif