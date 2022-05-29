import java.util.ArrayList;
import java.util.List;

// Data class Artist for storing data about specific member of a group
public class Artist {
    private final int ID;
    private final String name;
    private final List<String> groups;

    Artist(int ID, String name) {
        this.ID = ID;
        this.name = name;
        groups = new ArrayList<>();
    }

    public int getID() {
        return ID;
    }

    public String getName() {
        return name;
    }

    public void addGroup(String groupName) {
        groups.add(groupName);
    }

    public String getGroups() {
        return groups.toString();
    }

    public boolean isPlayedInGroup(String groupName) {
        return groups.contains(groupName);
    }

    @Override
    public String toString() {
        return "Artist{" +
                "ID=" + ID +
                ", name='" + name + '\'' +
                '}';
    }
}