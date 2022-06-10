package discogs;

import URLCreator.ArtistIdURL;
import URLCreator.ArtistMembersURL;
import URLCreator.ArtistReleaseURL;

import java.util.List;
import java.util.Set;

public class InformationGetter {
    private final URLHandler urlHandler = new URLHandler();
    private final DiscogsParser parser = new DiscogsParser();

    public String getReleases(String artist) {
        String releaseInformation = urlHandler.downloadInformation(artist, new ArtistReleaseURL());
        return parser.parseReleases(releaseInformation, artist);
    }

    public String getArtistID(String artist, boolean isID) {
        // If id is already provided we don't need to search it
        if (isID) {
            return artist;
        }
        String artistInformation = urlHandler.downloadInformation(artist, new ArtistIdURL());
        return parser.parseArtistID(artistInformation, artist);
    }

    public List<Artist> getMembersOfGroup(String artistID) {
        String membersInformation = urlHandler.downloadInformation(artistID, new ArtistMembersURL());
        return parser.parseMembers(membersInformation);
    }

    public void getMemberHistory(Artist artist, Set<String> globalGroups) {
        // Performing sleep to prevent 429 (To many requests) response code
        try {
            System.out.print('.');
            Thread.sleep(3000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        String memberHistory = urlHandler.downloadInformation(String.valueOf(artist.getID()), new ArtistMembersURL());
        parser.parseMemberHistory(memberHistory, artist, globalGroups);
    }
}
