CC = gcc
CFLAGS = -pedantic -Wall -std=c99


all: client server 
	
client: sumClient.c functions.c
	$(CC) $(CFLAGS) sumClient.c -o sumClient functions.c

server: sumServer.c functions.c
	$(CC) $(CFLAGS) sumServer.c -o sumServer functions.c

.Phony:
	clean

clean:
	rm -f sumClient sumServer