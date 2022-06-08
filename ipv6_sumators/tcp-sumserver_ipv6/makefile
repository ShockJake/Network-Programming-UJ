CC = gcc
S_FLAGS = -pedantic -Wall -pthread
C_FLAGS = -pedantic -Wall

all: sumServer.c client.c functions.h functions.c
	$(CC) $(S_FLAGS) sumServer.c -o sumServer functions.c
	$(CC) $(C_FLAGS) client.c -o client

server: sumServer.c functions.h functions.c
	$(CC) $(S_FLAGS) sumServer.c -o sumServer functions.c

client: client.c
	$(CC) $(C_FLAGS) client.c -o client

.PHONY:
	clear

clear:
	rm -f sumServer
	rm -f client