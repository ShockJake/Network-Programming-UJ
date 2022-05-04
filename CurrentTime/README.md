# Parser of current Time in Poland

Program that shows current time in Poland by making a HTTPS request on timeanddate.com/worldclock/poland page,
and finds a time counter on that page. By default uses a Jsoup library to parse page.

## Compiling

> $ javac -cp jsoup-1.14.3.jar CurrentTimePL.java

## Running

> $ java -cp jsoup-1.14.3.jar:. CurrentTime '-o' '-d'

Optional flags:
- -o - Old search type (using manual search). In other case the Jsoup parsing will be used.
- -d - Debug mode flag. Shows general information and describes whether some error occured.

## Files

### lib

Folder for libraries

> Jsoup used in this project

### src

> Folder for source code

### bin

> Folder for output classes

