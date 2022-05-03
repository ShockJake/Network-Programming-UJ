import javax.net.ssl.HttpsURLConnection;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class HttpChecker {
    static BufferedReader bufferedReader = null;
    static int isDebugMode = 0;

    public static void main(String[] args) throws IOException {

        // Checking given arguments
        checkInput(args);

        // Url we need to check
        String inputUrl = args[0];
        // Phrase we need to find on the page
        String checkPhrase = args[1];

        URL url = new URL(inputUrl);
        HttpsURLConnection connection = (HttpsURLConnection) url.openConnection();

        // Checking whether http request had an ok response
        if (connection.getResponseCode() != HttpURLConnection.HTTP_OK) {
            if (isDebugMode > 0) {
                System.out.println("No HTTP_OK response");
            }
            System.exit(1);
        }

        // Checking if type of page content is html
        if (!connection.getContentType().equals("text/html")) {
            if (isDebugMode > 0) {
                System.out.println("Page content is not 'text/html'");
            }
            System.exit(1);
        }

        // Reader for reading the internet page
        bufferedReader = new BufferedReader(new InputStreamReader(connection.getInputStream()));

        // Boolean variable to check whether given header phrase occurs on the page
        boolean phraseIsFound = false;
        String s;
        while ((s = bufferedReader.readLine()) != null) {
            if (s.startsWith("<h")) {
                if (isDebugMode == 2) {
                    System.out.println(s);
                }
                if (checkLine(s, checkPhrase)) {
                    phraseIsFound = true;
                }
            }
        }
        bufferedReader.close();

        if (!phraseIsFound) {
            if (isDebugMode > 0) {
                System.out.println("Phrase is not found");
            }
            System.exit(1);
        }
        if (isDebugMode > 0) {
            System.out.println("Page works...");
        }
        System.exit(0);
    }
    static private boolean checkLine(String line, String phrase) {
        String subStrLine = line.substring(4, line.length() - 4);
        if (isDebugMode == 2) {
            System.out.println("[+] - " + subStrLine);
        }
        return subStrLine.startsWith(phrase);
    }

    static private void checkInput(String[] args) {
        // Checking number of arguments
        if (args.length < 2 || args.length > 3) {
            printUsage();
            System.exit(1);
        }
        // Checking whether debug mode is turned on.
        if (args.length == 3) {
            if (args[2].equals("-d")) {
                isDebugMode = 1;
            }
            if (args[2].equals("-D")) {
                isDebugMode = 2;
            }
            else {
                printUsage();
            }
        }
    }

    static private void printUsage() {
        System.out.println("Usage:");
        System.out.println("./java HttpChecker <URL> <PHRASE> < -d >");
        System.out.println("\t* URL - page you want to check");
        System.out.println("\t* PHRASE - words you want to find on a page to check if it's really works");
        System.out.println("\t* -d - debug mode flag (Use if you want to get information about errors)");
    }
}
