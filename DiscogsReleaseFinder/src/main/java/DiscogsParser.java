import org.json.simple.parser.ParseException;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class DiscogsParser implements Parser {

    private String verifyResponse(HttpURLConnection connection) throws IOException {
        int responseCode = connection.getResponseCode();
        if (responseCode != 200) {
            return String.format("Bad response code: %s", responseCode);
        }
        if (!connection.getContentType().equals("application/json")) {
            return String.format("Non valid content/type: %s", connection.getContentType());
        }
        return "OK";
    }

    public static String getResponseContent(HttpURLConnection connection) throws IOException {
        StringBuilder builder = new StringBuilder();
        BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String line;
        while ((line = reader.readLine()) != null) {
            builder.append(line);
        }
        reader.close();
        return builder.toString();
    }

    @Override
    public String getArtistReleases(String artist) {
        try {
            URL url = new URL(DiscogsUtil.getArtistAlbums(artist));
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");
            String responseInformation = verifyResponse(connection);

            if (responseInformation.equals("OK")) {
                String artistWithUC = DiscogsUtil.checkArtist(artist);
                String result = DiscogsUtil.parseReleases(getResponseContent(connection), artistWithUC);
                if (result.length() != 0) {
                    return "For provided artist (" + artistWithUC + ") found next releases:\n" + result;
                } else {
                    return "Can't find any releases for provided artist: " + artist;
                }
            } else {
                return responseInformation;
            }
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException | ParseException e) {
            throw new RuntimeException(e);
        }
        return "Failure occurred";
    }

    @Override
    public String getGroupMembersHistory(String artist) {
        try {
            URL urlArtistID = new URL(DiscogsUtil.getArtistID(artist));
            HttpURLConnection connection = (HttpURLConnection) urlArtistID.openConnection();
            connection.setRequestMethod("GET");
            String responseInformation = verifyResponse(connection);
            if (responseInformation.equals("OK")) {
                String artistID = DiscogsUtil.parseArtistID(getResponseContent(connection));
                URL urlMembers = new URL(DiscogsUtil.getArtistMembers(artistID));
                connection = (HttpURLConnection) urlMembers.openConnection();
                connection.setRequestMethod("GET");
                return DiscogsUtil.parseMembers(getResponseContent(connection)).toString();
            } else {
                return responseInformation;
            }
        } catch (IOException | ParseException e) {
            e.printStackTrace();
        }
        return "Failure occurred";
    }
}
