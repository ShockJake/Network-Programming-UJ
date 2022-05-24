# Musical releases finder

Program that parses artist's releases, for provided name od artist.
Using Discogs REST API. 

After running program providing artist name as input argument program sends
**GET** request to Discogs website, where Discogs API searches required data and
sends a JSON file with list of releases, next step - program *filters* data and 
writes result to the **stdout**.

Libraries used:
-

- **JSON simple** - *for parsing JSON files*.

## Compilation

> $ java -cp json-simple-1.1.1.jar DiscogsParser.java

## Running

> $ java -cp json-simple-1.1.1.jar:. DiscogsParser "*artist name*"

