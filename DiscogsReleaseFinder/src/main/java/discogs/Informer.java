package discogs;

public interface Informer {
    String getArtistReleases(String artist);
    String getGroupMembersHistory(String artist, boolean isID);
}
