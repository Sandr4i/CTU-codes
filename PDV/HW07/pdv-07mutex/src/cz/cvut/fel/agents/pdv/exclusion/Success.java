package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;

public class Success extends ClockedMessage {
    public String section;

    public Success(String section) {
        this.section = section;
    }
}
