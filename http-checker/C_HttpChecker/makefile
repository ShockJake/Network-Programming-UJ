CC = gcc
CFLAGS = -Wall -pedantic

all:
	$(CC) $(CFLAGS) http_checker.c -o http_checker -lcurl

.Phony:
	clean

clean:
	rm -f http_checker