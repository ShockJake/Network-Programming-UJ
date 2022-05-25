import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.util.*;

public class DiscogsUtil {
    // Token for the discogs page
    private static final String token = "tFFpfMGVXnlmcvflRBArACvhVuVGkFhJmMWsDrnk";

    // Methods to generate url for REST requests:
    public static String getArtistID(String artist) {
        return String.format("https://api.discogs.com/database/search?q=%s&token=%s&?type=artist",
                artist, token);
    }

    public static String getArtistAlbums(String artist) {
        return String.format("https://api.discogs.com/database/search?q=%s&token=%s&type=master", artist, token);
    }

    public static String getArtistMembers(String artistID) {
        return String.format("https://api.discogs.com/artists/%s", artistID);
    }

    public static String checkArtist(String artist) {
        String artistWithUC = "";
        if (artist.charAt(0) > 96 && artist.charAt(0) < 122) {
            artistWithUC += Character.toUpperCase(artist.charAt(0));    // If first letter of provided artist was in
            artistWithUC += artist.substring(1);   // lower case, performing toUpperCase method
        } else {                                                        // to perform search in result, otherwise just
            artistWithUC = artist;                                      // passing given artist name value.
        }
        return artistWithUC;
    }

    private static String sortReleases(Map<Integer, String> releases) {
        Map<Integer, String> treeMap = new TreeMap<>(releases);
        StringBuilder builder = new StringBuilder();
        for (Integer i : treeMap.keySet()) {
            builder.append('\t').append(treeMap.get(i)).append(" ").append(i).append('\n');
        }
        return builder.toString();
    }

    public static String parseReleases(String file, String artist) throws ParseException {
        Map<Integer, String> map = new HashMap<>();
        JSONObject response = (JSONObject) new JSONParser().parse(file);  // Parsing response as a JSONObject
        JSONArray array = (JSONArray) response.get("results");            // Parsing results from it as a JSONArray
        for (Object el : array) {
            JSONObject object = (JSONObject) el; // Parsing release as JSONObject
            String title = object.get("title").toString();
            Object year = object.get("year");
            if (year == null) {
                continue;
            }
            // Placing title and year to a map if it is about desired artist
            if (title.startsWith(artist)) {
                map.put(Integer.parseInt(year.toString()), title.substring(title.indexOf("-") + 1));
            }
        }
        return sortReleases(map);
    }

    public static String parseArtistID(String file) throws ParseException {
        // Parsing id of artist
        JSONObject response = (JSONObject) new JSONParser().parse(file);
        JSONArray array = (JSONArray) response.get("results");
        JSONObject artistJSON = (JSONObject) array.get(0);

        String artistName = artistJSON.get("title").toString();
        String artistID = artistJSON.get("id").toString();
        // Debug information
        System.out.println("Artist was found successfully name=" +
                artistName + ", id=" + artistID + '\n');
        return artistID;
    }

    public static List<String> parseMembers(String file) throws ParseException {
        JSONObject object = (JSONObject) new JSONParser().parse(file);

        JSONArray members = (JSONArray) object.get("members");
        List<String> membersList = new ArrayList<>();

        for (Object member : members) {
            JSONObject memberObj = (JSONObject) member;
            membersList.add(memberObj.get("name").toString());
        }
        return membersList;
    }
}
