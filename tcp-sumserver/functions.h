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

// Function to check if the characters represents number
bool is_number(char input, unsigned short int *space_counter);
// Function to send an error to the client
void send_error(int asd);
// Function to add "\r\n" at the end of message
void add_ending(char *data);
// Function to create socket
int create_socket(int port);
// Function to close server socket
void close_server(int server_descriptor);
// Function to fill the server sockaddr struct
void fill_sockaddrs_struct(int port, struct sockaddr_in *addr);
// Function to bind socket and make it listening to the new connections
void bind_and_listen(int server_descriptor, const struct sockaddr *addr);
// Function to close connection with client
int close_connection(int con);
// Function to sum numbers that contained in message
unsigned long long int sum_numbers(char *data);
// Function to send answer from server to client
void send_data(unsigned long long int answer_int, int clientDescriptor);
// Function to verify that given data is acceptable by a server
bool check_data(char *message);
// Function to communicate with client
bool perform_action(unsigned long long int *number, int cd);
// Function to show address of new client that is connected
void show_new_client(int client_descriptor, char *client_addr, int port);
// Set timeout
int set_timeout(int socket);

#endif // FUNCTIONS_H