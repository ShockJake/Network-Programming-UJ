# Sum Server / Sum Client

Programs to implement Server takes numbers as input and returns sum of all that numbers

## Compiling

> $ gcc -std=c99 -pedantic -Wall sumServer.c functions.c -o sumServer

## Running

> $ ./sumServer 2020

## Containment:

- functions.h

Header file for all functions such as: creation of socket, starting server, checking input.

- functions.c

File that contains implementation of all functions.

- sumServer.c

File that creates and runs server.

- sumClient.c

File that creates and runs client.