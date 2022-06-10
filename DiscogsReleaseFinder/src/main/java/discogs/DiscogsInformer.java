package discogs;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class DiscogsInformer implements Informer {
    private final InformationGetter getter = new InformationGetter();

    private static String checkArtist(String artist) {
        String artistWithUC = "";
        if (artist.charAt(0) > 96 && artist.charAt(0) < 122) {
            artistWithUC += Character.toUpperCase(artist.charAt(0));    // If first letter of provided artist was in
            artistWithUC += artist.substring(1);               // lower case, performing toUpperCase method
        } else {                                                        // to perform search in result, otherwise just
            artistWithUC = artist;                                      // passing given artist name value.
        }
        return artistWithUC;
    }

    @Override
    public String getArtistReleases(String artist) {
        return getter.getReleases(checkArtist(artist));
    }

    // Method to check if group contains more than one specific artist
    private boolean checkGroupMembers(String line) {
        String[] members = line.substring(line.indexOf(':') + 2).split(" ");
        return members.length > 3;
    }

    private String getMembersHistoryResult(List<String> groups, String artist) {
        StringBuilder builder = new StringBuilder();
        for (int i = 0; i < groups.size(); i++) {                           //
            String group = groups.get(i);                                   //
            for (int j = 0; j < groups.size(); j++) {                       //
                String g = groups.get(j);                                   //
                if (g.startsWith(group) && g.length() > group.length()) {   // Removing repeating groups
                    groups.remove(group);                                   //
                    i = 0;                                                  //
                }                                                           //
            }                                                               //
        }
        for (String group : groups) {
            if (group.startsWith(' ' + artist)) {
                continue;
            }
            builder.append(
                    group.replace(",", "\n\t").replace(":", ":\n\t")).append('\n');
        }
        return builder.toString();
    }

    private String getGroups(List<Artist> members, Set<String> globalGroups, String artistName) {
        List<String> groups = new ArrayList<>();
        StringBuilder builder = new StringBuilder();
        for (String group : globalGroups) {                         //
            builder.append(group).append(": ");                     // Adding group name
            for (Artist artist : members) {                         //
                if (artist.getGroups().contains(group)) {           //
                    builder.append(artist.getName()).append(", ");  //
                    if (checkGroupMembers(builder.toString())) {    // If there are more than one member,
                        groups.add(builder.toString());             // adding group to the list
                    }                                               //
                }                                                   //
            }                                                       //
            builder.delete(0, builder.length() - 1);                // deleting StringBuilder for the next group
        }                                                           //
        return getMembersHistoryResult(groups, artistName);
    }

    @Override
    public String getGroupMembersHistory(String artist, boolean isID) {
        String artistID = getter.getArtistID(artist, isID);
        List<Artist> members = getter.getMembersOfGroup(artistID);
        Set<String> globalGroups = new HashSet<>();

        System.out.print("Processing");
        for (Artist member : members) {
            getter.getMemberHistory(member, globalGroups);
        }
        System.out.println("\n Groups:");
        return getGroups(members, globalGroups, artist);
    }
}
