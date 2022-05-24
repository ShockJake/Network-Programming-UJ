import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class DiscogsParser {
    // Token for the discogs page
    private static final String token = "tFFpfMGVXnlmcvflRBArACvhVuVGkFhJmMWsDrnk";

    // Method to generate url for REST request
    private static String getArtistAlbums(String artist) {
        return String.format("https://api.discogs.com/database/search?q=%s&token=%s&type=master", artist, token);
    }

    private static String verifyResponse(HttpURLConnection connection) throws IOException {
        int responseCode = connection.getResponseCode();
        if (responseCode != 200) {
            return String.format("Bad response code: %s", responseCode);
        }
        if (!connection.getContentType().equals("application/json")) {
            return String.format("Non valid content/type: %s", connection.getContentType());
        }
        return "OK";
    }

    private static void printUsage() {
        System.out.println("Usage:\n\tjava DiscogsParser <artist name>");
    }

    private static String getResponseContent(HttpURLConnection connection) throws IOException {
        StringBuilder builder = new StringBuilder();
        BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String line;
        while ((line = reader.readLine()) != null) {
            builder.append(line);
        }
        return builder.toString();
    }

    private static String parseResponse(String file, String artist) throws ParseException {
        String artistWithUL = "";
        if (artist.charAt(0) > 96 && artist.charAt(0) < 122) {
            artistWithUL += Character.toUpperCase(artist.charAt(0));    // If first letter of provided artist was in
            artistWithUL += artist.substring(1, artist.length() - 1);   // lower case, performing toUpperCase method
        } else {                                                        // to perform search in result, otherwise just
            artistWithUL = artist;                                      // passing given artist name value.
        }
        JSONObject object = (JSONObject) new JSONParser().parse(file);  // Parsing response as a JSONObject
        JSONArray array = (JSONArray) object.get("results");            // Parsing results from it as a JSONArray
        StringBuilder builder = new StringBuilder();
        for (Object el : array) {
            JSONObject object1 = (JSONObject) el; // Parsing release as JSONObject
            // Preparing a string for printing such way: "(Release name) (Year of release)"
            String line = String.format("%s %s\n", object1.get("title").toString(), object1.get("year"));
            // Check if line contains desired artist
            if (line.startsWith(artistWithUL) && !line.contains("null")) {
                builder.append('\t').append(line.substring(line.indexOf('-') + 1));
            }
        }
        return builder.toString();
    }

    public static void main(String[] args) {
        if (args.length != 1) {
            printUsage();
            System.exit(1);
        }
        // Getting artist name form input argument
        String artist = args[0];
        try {
            URL url = new URL(getArtistAlbums(artist));
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");
            String response = verifyResponse(connection);
            if (response.equals("OK")) {
                String result = parseResponse(getResponseContent(connection), artist);
                if (result.length() != 0) {
                    System.out.println("For provided artist (" + artist + ") found next releases:");
                    System.out.println(result);
                } else {
                    System.out.println("Can't find any releases for provided artist: " + artist);
                }
            } else {
                System.out.println(response);
            }
        } catch (IOException | ParseException e) {
            e.printStackTrace();
        }
    }
}
