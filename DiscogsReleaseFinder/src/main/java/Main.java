public class Main {

    private static void printUsage() {
        System.out.println("""
                "Usage:
                \tjava DiscogsParser <artist_name>""");
    }

    public static void main(String[] args) {

        if (args.length != 1) {
            printUsage();
            System.exit(1);
        }
        String artist = args[0];
        Parser parser = new DiscogsParser();
        System.out.println(parser.getArtistReleases(artist));
        System.out.println(parser.getGroupMembersHistory(artist));
    }
}
