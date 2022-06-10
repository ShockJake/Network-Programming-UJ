# Discogs discogs.Informer

Program that parses artist's releases and groups in which all members played,
for provided name od artist. Using Discogs REST API.

After running program and providing artist name (or artist ID for members history)
as input argument program sends **GET** requests to Discogs website, where Discogs API searches required data and
sends a **JSON** file with list of releases or members of specific group,
next step - program *filters* data and writes result to the **stdout**.

### Project structure:

- **URLCreator** - package for URL creators, classes that construct a specific url for
  REST API of the Discogs website (ArtistID, ArtistMember, ArtistRelease - classes
  for a different urls)

- **discogs.Artist** - data class for an discogs.Artist or a member of a group.

- **discogs.URLHandler** - class for downloading, verifying and getting response from URLs

- **discogs.DiscogsParser** - class that reads _JSON_ files and converts them into program
  readable information.

- **Information Getter** - class for getting information to proceed, includes
  checks and functionality with handling results of getting url and parsing.

- **discogs.DiscogsInformer** - class that searches and sorts data got form Discogs website

- **discogs.Informer** - interface for using discogs.DiscogsInformer

[Jar file](https://github.com/ShockJake/Network-Programming-UJ/raw/main/DiscogsReleaseFinder/out/artifacts/DiscogsInformer_jar/DiscogsInformer.jar "Jar")
for downloading.

Libraries used:
-

- **JSON simple** - *for parsing JSON files*.

## Compilation

> $ javac -cp json-simple-1.1.1.jar discogs.DiscogsInformer.java

## Running

> $ java -cp json-simple-1.1.1.jar:. discogs.DiscogsInformer "*artist name*" "*flags...*"

If you are using jar file:

> $ java -cp discogs.DiscogsInformer.jar "*artist name*" "*flags...*"

Flags:

- -r - get discogs.Artist Releases

- -h - get Members History

- -ID - you provide ID instead of discogs.Artist name (only for Members History mode)

## Example of use

```Java
import discogs.DiscogsInformer;
import discogs.Informer;

class Program {
    public static void main(String[] args) {
        String artist = "nirvana";
        boolean isID = false;

        Informer informer = new DiscogsInformer();
        System.out.println(informer.getArtistReleases(artist));
        System.out.println(informer.getGroupMembersHistory(artist, isID));
    }
}
```