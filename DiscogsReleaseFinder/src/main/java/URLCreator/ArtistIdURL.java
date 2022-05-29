package URLCreator;

public class ArtistIdURL implements URLCreator{
    @Override
    public String createURL(String parameter) {
        return String.format("https://api.discogs.com/database/search?q=%s&token=%s&?type=artist",
                parameter, token);
    }
}
