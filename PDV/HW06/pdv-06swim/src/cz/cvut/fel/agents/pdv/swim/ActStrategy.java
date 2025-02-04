package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;

import java.util.*;

import static java.lang.Math.*;

/**
 * Trida s implementaci metody act() pro proces Failure Detector. Tuto tridu (a tridy pouzivanych zprav) budete
 * odevzdavat. Do tridy si muzete doplnit vlastni pomocne datove struktury. Hodnoty muzete inicializovat primo
 * v konstruktoru. Klicova je metoda act(), kterou vola kazda instance tridy FailureDetectorProcess ve sve metode
 * act(). Tuto metodu naimplementujte podle protokolu SWIM predstaveneho na prednasce.
 *
 * Pokud si stale jeste nevite rady s frameworkem, inspiraci muzete nalezt v resenych prikladech ze cviceni.
 */
public class ActStrategy {

    // maximalni zpozdeni zprav
    private final int maxDelayForMessages;
    private final List<String> otherProcesses;
    private final int upperBoundOnMessages;
    private final int timeToDetectKilledProcess;

    int messagesSent ;
    private int tickCounter;
    private int currentIndex;
    private final HashMap<String, Boolean> pings;
    private final HashMap<String, HashSet<String>> callbacks;
    private final HashMap<String, Integer> lastSend;
    private final Random rand;

    // Definujte vsechny sve promenne a datove struktury, ktere budete potrebovat

    public ActStrategy(int maxDelayForMessages, List<String> otherProcesses,
                       int timeToDetectKilledProcess, int upperBoundOnMessages) {
        this.maxDelayForMessages = maxDelayForMessages;
        this.timeToDetectKilledProcess = timeToDetectKilledProcess;
        this.upperBoundOnMessages = upperBoundOnMessages;
        Collections.shuffle(otherProcesses);
        this.otherProcesses = otherProcesses.subList(0, 25);

        this.pings = new HashMap<>();
        this.callbacks = new HashMap<>();
        this.lastSend = new HashMap<>();
        this.rand = new Random();
        this.tickCounter = 0;
        this.currentIndex = -1;
        messagesSent = 0;

        // Doplne inicializaci
    }

    private String selectNextNeighbor(){
        int size = this.otherProcesses.size();
        int index = (this.currentIndex + 1) % size;

        for (int i = 0; i < size; i++){
            int idx = (index + i) % size;
            if (!this.pings.containsKey(otherProcesses.get(idx))){
                this.currentIndex = idx;
                return this.otherProcesses.get(idx);
            }
        }

        return null;
//        while(this.pings.containsKey(this.otherProcesses.get(index))){
//            index = (index+1) % size;
//        }
//        this.currentIndex = index;
//        return this.otherProcesses.get(this.currentIndex);
    }

    /**
     * Metoda je volana s kazdym zavolanim metody act v FailureDetectorProcess. Metodu implementujte
     * tak, jako byste implementovali metodu act() v FailureDetectorProcess, misto pouzivani send()
     * pridejte zpravy v podobe paru - prijemce, zprava do listu. Zpravy budou nasledne odeslany.
     * <p>
     * Diky zavedeni teto metody muzeme kontrolovat pocet odeslanych zprav vasi implementaci.
     */

    public List<Pair<String, Message>> act(Queue<Message> inbox, String disseminationProcess) {
        List<Pair<String, Message>> output = new ArrayList<>();


        while (!inbox.isEmpty() && this.messagesSent < upperBoundOnMessages) {
            Message msg = inbox.poll();
            if (this.currentIndex == -1) this.currentIndex = this.otherProcesses.indexOf(msg.recipient);
            if (msg instanceof PingMessage) {
                PingMessage ping = (PingMessage) msg;
                if (this.messagesSent < this.upperBoundOnMessages){
                    output.add(new Pair<>(ping.sender, new AckMessage(ping.getProcessID())));
                    this.messagesSent++;
                }
                if (this.callbacks.containsKey(ping.sender)){
                    for (String writer : this.callbacks.get(ping.sender)){
                        if (this.messagesSent < this.upperBoundOnMessages){
                            output.add(new Pair<>(writer, ping));
                            this.messagesSent++;
                        }
                    }
                }
                this.lastSend.remove(ping.getProcessID());
                this.pings.remove(ping.getProcessID());
            }
            else if (msg instanceof PingRequest){
                PingRequest ping = (PingRequest) msg;
                if (this.messagesSent < this.upperBoundOnMessages){
                    output.add(new Pair<>(ping.getProcessID(), new PingMessage(ping.getProcessID())));
                    this.messagesSent++;
                }
                if(!this.callbacks.containsKey(ping.getProcessID())){
                    this.callbacks.put(ping.getProcessID(), new HashSet<>());
                }

                this.callbacks.computeIfAbsent(ping.getProcessID(), k->new HashSet<>()).add(ping.sender);
            }
            else if (msg instanceof AckMessage){
                AckMessage ack = (AckMessage) msg;
                if (this.callbacks.containsKey(ack.getProcessID())){
                    for (String writer : this.callbacks.get(ack.getProcessID())){
                        if (this.messagesSent < this.upperBoundOnMessages){
                            output.add(new Pair<>(writer, ack));
                            this.messagesSent++;
                        }
                        else break;
                    }
                }
                this.lastSend.remove(ack.getProcessID());
                this.pings.remove(ack.getProcessID());
            }
            else if (msg instanceof DeadProcessMessage){
                DeadProcessMessage dead = (DeadProcessMessage) msg;
                this.otherProcesses.remove(dead.getProcessID());
            }
        }

        //if (messagesSent >= this.upperBoundOnMessages) return new ArrayList<>(new HashSet<>(output));

        if (this.tickCounter % 2 == 1 && this.messagesSent < this.upperBoundOnMessages){
            String target = selectNextNeighbor();
            output.add(new Pair<>(target, new PingMessage(target)));
            this.messagesSent++;
            this.lastSend.put(target, this.tickCounter);
        }

        List<String> suspects = new ArrayList<>();
        for (Map.Entry<String, Integer> entry : this.lastSend.entrySet()) {
            if (entry.getValue() < this.tickCounter - 2 * this.maxDelayForMessages
                    && !this.pings.containsKey(entry.getKey())) {
                if (this.messagesSent < this.upperBoundOnMessages){
                    List<String> randomSubSet = new ArrayList<>(this.otherProcesses);
                    Collections.shuffle(randomSubSet, this.rand);

                    for (int i = 0; i < 8; i++){
                        if (this.messagesSent < this.upperBoundOnMessages){
                            output.add(new Pair<>(randomSubSet.get(i), new PingRequest(entry.getKey())));
                            this.messagesSent++;
                        }
                        else break;
                    }
                    this.pings.put(entry.getKey(), true);
                }
            }
            if (entry.getValue() < this.tickCounter - 5 * this.maxDelayForMessages){
                if (this.messagesSent < this.upperBoundOnMessages){
                    output.add(new Pair<>(disseminationProcess, new PFDMessage(entry.getKey())));
                    this.messagesSent++;
                }
                suspects.add(entry.getKey());
            }
        }

        for (String dead : suspects){
            this.lastSend.remove(dead);
            this.pings.remove(dead);
        }
        this.tickCounter++;

//        System.out.println("Messegaes sent: " + this.messagesSent);
//        System.out.println("Upper bounds: " + this.upperBoundOnMessages);
        return new ArrayList<>(new HashSet<>(output));
        // Od DisseminationProcess muzete dostat zpravu typu DeadProcessMessage, ktera Vas
        // informuje o spravne detekovanem ukoncenem procesu.
        // DisseminationProcess muzete poslat zpravu o detekovanem "mrtvem" procesu.
        // Zprava musi byt typu PFDMessage.
    }

    public static class PingMessage extends Message {

        private final String processID;

        public PingMessage(String processID) {
            this.processID = processID;
        }

        public String getProcessID() {
            return this.processID;
        }
    }

    public static class AckMessage extends Message {

        private final String processID;

        public AckMessage(String processID) {
            this.processID = processID;
        }

        public String getProcessID() {
            return this.processID;
        }
    }

    public static class PingRequest extends Message {

        private final String processID;

        public PingRequest(String processID) {
            this.processID = processID;
        }

        public String getProcessID() {
            return this.processID;
        }
    }

}