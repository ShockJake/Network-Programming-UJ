import discogs.DiscogsInformer;
import discogs.Informer;

public class Main {

    private static void printUsage() {
        System.out.println("""
                "Usage:
                \tjava DiscogsParser <artist_name> <flags>
                Flags:
                \t-r - get Artist Releases
                \t-h - get Members History
                \t-ID - you provide ID instead of Artist name (only for Members History mode)""");
    }

    private static void performFail(String error) {
        System.out.println("Error: " + error);
        printUsage();
        System.exit(1);
    }

    public static void main(String[] args) {
        boolean isID = false;
        boolean releasesMode = false;
        boolean membersHistoryMode = false;

        if (args.length < 2) {
            performFail("Not enough args");
        }
        for (int i = 1; i < args.length; i++) {
            if (args[i].equals("-r")) {
                releasesMode = true;
            } else if (args[i].equals("-ID")) {
                isID = true;
            } else if (args[i].equals("-h")) {
                membersHistoryMode = true;
            } else {
                performFail("Bad flag");
            }
        }
        if (releasesMode && membersHistoryMode) {
            performFail("Both release and members history modes enabled");
        }

        String artist = args[0];
        Informer informer = new DiscogsInformer();
        if (releasesMode) {
            System.out.println(informer.getArtistReleases(artist));
        }
        if (membersHistoryMode) {
            System.out.println(informer.getGroupMembersHistory(artist, isID));
        }

    }
}
