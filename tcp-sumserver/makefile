CC = gcc
S_FLAGS = -pedantic -Wall -pthread
C_FLAGS = -pedantic -Wall

all:
	$(CC) $(S_FLAGS) sumServer.c -o sumServer functions.c
	$(CC) $(C_FLAGS) client.c -o client

server:
	$(CC) $(S_FLAGS) sumServer.c -o sumServer functions.c

client:
	$(CC) $(C_FLAGS) client.c -o client

.PHONY:
	clear

clear:
	rm -f sumServer
	rm -f client