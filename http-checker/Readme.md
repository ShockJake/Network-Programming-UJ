# HTTP checker

Program to check whether internet page works.

Program checks next parameters:
- HTTP responce code
- Page content type
- Presence of given phrase

## Compiling C program

> $ make

## Compiling Java program

> $ javac HttpChecker.java

## Running

> $ ./HttpChecker /or/ java HttpChecker 'URL' 'PHRASE' '-d/D'

Parameters:
- URL - page you want to check
- PHRASE - words you want to find on page to verify it's content
- -d/D - debug mode flag (\n\tUse -d if you want to get information about error)
Use -D if you want to see each phrase check
