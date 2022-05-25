public class Main {

    private static void printUsage() {
        System.out.println("""
                "Usage:
                \tjava DiscogsParser <artist_name> <mode>
                - artist - name of artist you want to get information.
                - mode - what do you want to parse (1 - artist's releases, 2 - all members occurrences in other groups.
                (mode 1 is set by default)""");
    }

    public static void main(String[] args) {
        Parser parser = new DiscogsParser();
        System.out.println(parser.getArtistReleases("nirvana"));

        System.out.println(parser.getGroupMembersHistory("nirvana"));
    }
}
