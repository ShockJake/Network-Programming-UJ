CC = gcc
CFLAGS = -pedantic -Wall -std=c99

all: *.c
	$(CC) $(CFLAGS) sumServer.c -o sumServer functions.c
	$(CC) $(CFLAGS) sumClient.c -o sumClient functions.c

client: sumClient.c
	$(CC) $(CFLAGS) sumClient.c -o sumClient functions.c

server: sumServer.c
	$(CC) $(CFLAGS) sumServer.c -o sumServer functions.c

.Phony:
	clean

clean:
	rm -f sumClient sumServer