package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;

public class Request extends ClockedMessage {
    public String section;

    public Request(String section) {
        this.section = section;
    }

    @Override
    public String toString() {
        return "Message: " + this.section
                + "\nSend to: " + this.sentOn
                + "\nReceived: " + this.receivedOn
                + "\nSender: " + this.sender
                + "\nRecipient: " + this.recipient;
    }

    int sender(){
        int id = 0;
        String name = this.sender;
        for (Character c : name.toCharArray()) {
            if(c > '0' && c < '9') {
                id *= 10;
                id += Integer.parseInt(c.toString());
            }
        }
        return id;
    }

    int recipient(){
        int id = 0;
        String name = this.recipient;
        for (Character c : name.toCharArray()) {
            if(c < '0' || c > '9') continue;
            id *= 10;
            id += Integer.parseInt(c.toString());
        }
        return id;
    }
}
