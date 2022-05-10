import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;

import javax.net.ssl.HttpsURLConnection;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class CurrentTimePL {
    static private boolean debugMode = false;
    static private boolean oldMode = false;
    static private final String pageUrl = "https://www.timeanddate.com/worldclock/poland";

    private static void printUsage() {
        System.out.println("Usage:");
        System.out.println("./java -cp jsoup-1.14.3.jar:. CurrentTime '-o' '-d'");
        System.out.println("\t* -o - Old search flag (using manual search)");
        System.out.println("\t* -d - Debug mode flag\n");
    }

    // Method to check arguments given as input for program
    private static void checkInput(String[] args) {
        // Checking number of arguments
        if (args.length > 2) {
            printUsage();
            System.exit(1);
        }
        // Checking flags
        if (args.length > 0) {
            for (String arg : args) {
                // Checking flag for old search method
                if (arg.equals("-o")) {
                    oldMode = true;
                } else if (arg.equals("-d")) { // Checking flag for debug mode
                    debugMode = true;
                } else {
                    System.out.println("Bad flag given: " + arg);
                    printUsage();
                    System.exit(1);
                }
            }
        }
    }

    // Method to print given arguments if debug mode is turned on
    private static void printInput() {
        System.out.println("Input arguments:");
        System.out.println("URL - " + pageUrl);
        System.out.println("Debug mode - activated");
        if (oldMode) {
            System.out.println("Old search mode - activated\n");
        } else {
            System.out.println("New search mode - activated\n");
        }
    }

    // Method to check page response code and type of content
    private static boolean checkPage(HttpsURLConnection connection) throws IOException {
        // Checking whether http request had an HTTP_OK (200) response
        if (connection.getResponseCode() != HttpURLConnection.HTTP_OK) {
            if (debugMode) {
                System.out.println("No HTTP_OK response");
            }
            return false;
        }
        // Checking if type of page content is text/html
        String contentType = connection.getContentType();
        if (!contentType.startsWith("text/html")) {
            if (debugMode) {
                System.out.println("Page content is not 'text/html' - " + contentType);
            }
            return false;
        }
        return true;
    }

    // Method for getting time from web page manually by checking every line
    private static void getCurrentTimeOld(HttpsURLConnection connection, String checkLine) throws IOException {
        // Creating bufferedReader for reading page
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        // Flag to check whether phrase was found or not
        boolean phraseIsFound = false;
        String line;
        // Reading document
        while ((line = bufferedReader.readLine()) != null) {
            // Checking if line contains our checkLine
            if (line.contains(checkLine)) {
                // Finding indexes for time substring
                int startIndex = line.indexOf(checkLine);
                // Getting time as substring
                String time = line.substring(startIndex + 21, startIndex + 29);
                System.out.println("Current time: " + time);
                phraseIsFound = true;
                break;
            }
        }
        bufferedReader.close();
        if (!phraseIsFound) {
            System.out.println("Unable to find time");
            System.exit(1);
        }
    }

    // Method for getting time from web page using Jsoup
    private static void getCurrentTimeNew() throws IOException {
        // Parsing html document with Jsoup
        Document document = Jsoup.connect(CurrentTimePL.pageUrl).get();
        // Getting element that has time value
        Element element = document.getElementById("ct");
        if (element != null) {
            System.out.println("Current time: " + element.text());
        }
    }

    public static void main(String[] args) {
        // Checking given arguments
        checkInput(args);
        String checkLine = "<span id=ct";

        try {
            if (debugMode) {
                printInput();
            }
            URL url = new URL(pageUrl);
            HttpsURLConnection connection = (HttpsURLConnection) url.openConnection();
            // Verifying connection with page
            if (!checkPage(connection)) {
                System.out.println("Page state can't be verified");
                System.exit(1);
            }
            if (oldMode) {
                getCurrentTimeOld(connection, checkLine);
            } else {
                getCurrentTimeNew();
            }
        } catch (MalformedURLException e) {
            throw new RuntimeException(e);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
