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
int create_socket();

// Function to check if input is a number
bool is_number(char input);

// Function to check if input has some different data except from numbers
bool check_input(char *data, int length, bool *isRN);

// Server actions
unsigned long long int perform_action(char *data, int length);

// Function to create and start server
void start_sum_server(int port);

// Function to add a proper ending to the line
void add_ending(char *data);

// Function to send error message to the client
void send_error(char *msg, size_t msg_len, struct sockaddr *clientAddr, socklen_t len);

// Function to fill socket address structures
void fill_sock_addr_struct(struct sockaddr_in *server_addr, struct sockaddr_in *client_addr, int port);

// Function to bind socket
void bind_server_socket(struct sockaddr *server_addr);

// Function to clear buffers and variables
void clear_buffers_and_variables(char *answer, char *buff, char *result_char, bool *isRN);

// Function to receive info
int receive_data(char *buff, struct sockaddr *client_addr, socklen_t *len);

// Function to send data
void send_data(char *answer, struct sockaddr *client_addr, socklen_t *len);

// Function to convert int to char*
bool convert_result(char *answer, unsigned long long int *result, struct sockaddr *client_addr, char *error_msg);

// Function to close server socket
void close_server();

#endif