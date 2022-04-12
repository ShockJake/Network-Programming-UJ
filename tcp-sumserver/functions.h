#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>

bool isNumber(char input);
void sendError(int asd);
void addEnding(char *data);
int createSocket(int port);
unsigned long long int sumNumbers(char *data);
void sendData(unsigned long long int answer_int, int clientDescriptor);
bool performAction(unsigned long long int *number, int cd);

#endif // FUNCTIONS_H