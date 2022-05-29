package URLCreator;

public class ArtistMembersURL implements URLCreator{
    @Override
    public String createURL(String parameter) {
        return String.format("https://api.discogs.com/artists/%s", parameter);
    }
}
