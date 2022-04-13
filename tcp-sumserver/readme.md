# TCP Multithread Sum Server / Sum Client

Programs to implement Server takes numbers as input and returns sum of all that numbers

- Server uses a TCP protocol.
- Server can handle multiple clients at th esame time (multithreading is used).

## Compiling all

> $ make

## Compiling server

> $ make server

## Compiling client

> $ make client

## Running server

> $ ./sumServer >port<

By default port is set on 2020, but you can provide a specific port 
(note: if you want to provide a specific port, it must be higher than 1024).

## Containment:

- functions.h

Header file for all functions such as: creation of socket, sending data, checking input.

- functions.c

File that contains implementation of all functions.

- sumServer.c

File that creates and runs server.

- client.c

File that creates and runs client.