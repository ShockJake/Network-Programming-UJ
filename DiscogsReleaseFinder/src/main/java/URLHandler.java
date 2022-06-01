import URLCreator.URLCreator;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class URLHandler {

    public String verifyResponse(HttpURLConnection connection) throws IOException {
        int responseCode = connection.getResponseCode();
        if (responseCode != 200) {
            return String.format("Bad response code: %s", responseCode);
        }
        if (!connection.getContentType().equals("application/json")) {
            return String.format("Non valid content/type: %s", connection.getContentType());
        }
        return "OK";
    }

    public String getResponseContent(HttpURLConnection connection) throws IOException {
        StringBuilder builder = new StringBuilder();
        BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String line;
        while ((line = reader.readLine()) != null) {
            builder.append(line);
        }
        reader.close();
        return builder.toString();
    }

    public String downloadInformation(String artist, URLCreator creator) {
        try {
            // Getting url using polymorphic classes implementing URLCreator interface
            URL url = new URL(creator.createURL(artist));
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");
            String responseInformation = verifyResponse(connection);
            if (responseInformation.equals("OK")) {
                return getResponseContent(connection);
            } else {
                if (responseInformation.endsWith("429")) {
                    throw new RuntimeException("To many requests");
                }
                throw new RuntimeException(responseInformation);
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
