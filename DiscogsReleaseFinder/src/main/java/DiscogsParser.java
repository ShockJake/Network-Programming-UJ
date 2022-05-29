import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

public class DiscogsParser {

    private String sortReleases(Map<Integer, String> releases) {
        Map<Integer, String> treeMap = new TreeMap<>(releases);
        StringBuilder builder = new StringBuilder();
        for (Integer i : treeMap.keySet()) {
            builder.append('\t').append(treeMap.get(i)).append(" ").append(i).append('\n');
        }
        return builder.toString();
    }

    private void addReleasesToMap(JSONArray array, Map<Integer, String> releases, String artist) {
        for (Object release : array) {
            JSONObject object = (JSONObject) release;
            String title = object.get("title").toString();
            Object year = object.get("year");
            if (year == null) {
                continue;
            }
            if (title.startsWith(artist)) {
                releases.put(Integer.parseInt(year.toString()), title.substring(title.indexOf("-") + 1));
            }
        }
    }

    public String parseReleases(String file, String artist) {
        Map<Integer, String> releases = new HashMap<>();
        try {
            JSONObject response = (JSONObject) new JSONParser().parse(file);
            JSONArray array = (JSONArray) response.get("results");
            addReleasesToMap(array, releases, artist);
        } catch (ParseException e) {
            throw new RuntimeException(e);
        }
        String result = sortReleases(releases);
        if (result.length() != 0) {
            return "For provided artist (" + artist + ") found next releases:\n" + result;
        } else {
            throw new RuntimeException("Can't find any releases for provided artist: " + artist);
        }
    }

    private String findArtistID(JSONArray artists, String artistNameArg) {
        for (Object artistOBJ : artists) {
            JSONObject artist = (JSONObject) artistOBJ;
            if (artistNameArg.equals(artist.get("title").toString())) {
                String artistName = artist.get("title").toString();
                String artistID = artist.get("id").toString();
                System.out.println("Artist/Group was found successfully: name=" +
                        artistName + ", id=" + artistID + '\n');
                return artistID;
            }
        }
        throw new RuntimeException("Can't find artist with provided name: " + artistNameArg);
    }

    public String parseArtistID(String file, String artist) {
        try {
            JSONObject response = (JSONObject) new JSONParser().parse(file);
            JSONArray artists = (JSONArray) response.get("results");
            if (artists.size() == 0) {
                throw new RuntimeException("Can't find artist ID");
            }
            return findArtistID(artists, artist);
        } catch (ParseException e) {
            throw new RuntimeException(e);
        }
    }

    private void addMemberHistory(JSONArray groupsArray, Artist artist, Set<String> globalGroups) {
        for (Object memberObj : groupsArray) {
            JSONObject member = (JSONObject) memberObj;
            String name = member.get("name").toString();
            artist.addGroup(name);
            globalGroups.add(name);
        }
    }

    public void parseMemberHistory(String file, Artist artist, Set<String> globalGroups) {
        try {
            JSONObject object = (JSONObject) new JSONParser().parse(file);
            JSONArray groupsArray = (JSONArray) object.get("groups");
            addMemberHistory(groupsArray, artist, globalGroups);
        } catch (ParseException e) {
            throw new RuntimeException(e);
        }
    }

    public List<Artist> parseMembers(String file) {
        try {
            JSONObject object = (JSONObject) new JSONParser().parse(file);
            JSONArray members = (JSONArray) object.get("members");
            List<Artist> membersList = new ArrayList<>();
            for (Object member : members) {
                JSONObject memberObj = (JSONObject) member;
                Artist artist =
                        new Artist(Integer.parseInt(memberObj.get("id").toString()), memberObj.get("name").toString());
                membersList.add(artist);
            }
            return membersList;
        } catch (ParseException e) {
            throw new RuntimeException(e);
        }
    }
}
