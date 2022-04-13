CC = gcc
CFLAGS = -pedantic -Wall -pthread

all:
	$(CC) $(CFLAGS) sumServer.c -o sumServer functions.c

.PHONY:
	clear

clear:
	rm -f sumServer