package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;
import cz.cvut.fel.agents.pdv.clocked.ClockedProcess;

import java.util.*;

public class ExclusionPrimitive {

    /**
     * Stavy, ve kterych se zamek muze nachazet.
     */
    enum AcquisitionState {
        RELEASED,    // Uvolneny   - Proces, ktery vlastni aktualni instanci ExclusionPrimitive o pristup do kriticke
                     //              sekce nezada

        WANTED,      // Chteny     - Proces, ktery vlastni aktualni instanci ExclusionPrimitive zada o pristup do
                     //              kriticke sekce. Ten mu ale zatim nebyl odsouhlasen ostatnimi procesy.

        HELD         // Vlastneny  - Procesu bylo prideleno pravo pristupovat ke sdilenemu zdroji.
    }

    private ClockedProcess owner;            // Proces, ktery vlastni aktualni instanci ExclusionPrimitive

    private String criticalSectionName;      // Nazev kriticke sekce. POZOR! V aplikaci se muze nachazet vice kritickych
                                             // sekci s ruznymi nazvy!

    private String[] allAccessingProcesses;  // Seznam vsech procesu, ktere mohou chtit pristupovat ke kriticke sekci
                                             // s nazvem 'criticalSectionName' (a tak spolurozhoduji o udelovani pristupu)

    private AcquisitionState state;          // Aktualni stav zamku (vzhledem k procesu 'owner').
                                             // state==HELD znamena, ze proces 'owner' muze vstoupit do kriticke sekce

    // Doplnte pripadne vlastni datove struktury potrebne pro implementaci
    // Ricart-Agrawalova algoritmu pro vzajemne vylouceni
    private int idx;
    private Map<String, Boolean> successes;
    private int timeForRequest;
    private List <Request> requestList;

    public ExclusionPrimitive(ClockedProcess owner, String criticalSectionName, String[] allProcesses) {
        this.owner = owner;
        this.criticalSectionName = criticalSectionName;
        this.allAccessingProcesses = allProcesses;

        // Na zacatku je zamek uvolneny
        this.state = AcquisitionState.RELEASED;
        this.successes = new HashMap<>();
        this.requestList = new ArrayList<>();
        this.timeForRequest = 0;

        this.idx = 0;
        for (Character c : this.owner.id.toCharArray()) {
            if (c > '0' && c < '9') {
                this.idx *= 10;
                this.idx += Integer.parseInt(c.toString());
            }
        }
    }

    /**
     * Metoda pro zpracovani nove prichozi zpravy
     *
     * @param m    prichozi zprava
     * @return 'true', pokud je zprava 'm' relevantni pro aktualni kritickou sekci.
     */
    public boolean accept(ClockedMessage m) {

        // Implementujte zpracovani prijimane zpravy informujici
        // o pristupech ke sdilenemu zdroji podle Ricart-Agrawalova
        // algoritmu. Pokud potrebujete posilat specificke zpravy,
        // vytvorte si pro ne vlastni tridy.
        //
        // POZOR! Ne vsechny zpravy, ktere v teto metode dostanete Vas
        // budou zajimat! Budou Vam prichazet i zpravy, ktere se  napriklad
        // tykaji jinych kritickych sekci. Pokud je zprava nerelevantni, tak
        // ji nezpracovavejte a vratte navratovou hodnotu 'false'. Nekdo jiny
        // se o ni urcite postara :-)
        //
        // Nezapomente se starat o cas procesu 'owner'
        // pomoci metody owner.increaseTime(). Aktualizaci
        // logickeho casu procesu s prijatou zpravou (pomoci maxima) jsme
        // za Vas jiz vyresili.
        //
        // Cas poslani prijate zpravy muzete zjistit dotazem na hodnotu
        // m.sentOn. Aktualni logicky cas muzete zjistit metodou owner.getTime().
        // Zpravy se posilaji pomoci owner.send(prijemce, zprava) a je jim auto-
        // maticky pridelen logicky cas odeslani. Retezec identifikujici proces
        // 'owner' je ulozeny v owner.id.

        if(m instanceof Request) {
            Request request = (Request) m;
            if(!request.section.equals(this.criticalSectionName)) {
                return false;
            }

            if(this.state == AcquisitionState.HELD
                    || (this.state == AcquisitionState.WANTED
                    && (this.timeForRequest < request.sentOn
                    || (this.timeForRequest == request.sentOn
                    && this.idx < request.sender())))) {
                requestList.add(request);
            }
            else{
                this.owner.send(request.sender, new Success(this.criticalSectionName));
            }
            return true;
        }

        if (m instanceof Success) {
            Success success = (Success) m;
            if (!this.criticalSectionName.equals(success.section)) {
                return false;
            }

            this.successes.put(success.sender, true);
            boolean hasAll = true;
            for (Map.Entry<String, Boolean> entry : this.successes.entrySet()) {
                if (!entry.getValue()) {
                    hasAll = false;
                    break;
                }
            }
            if(hasAll){
                this.owner.increaseTime();
                this.state = AcquisitionState.HELD;
            }
            return true;
        }
        return false;
        //throw new RuntimeException("Not implemented yet");

    }

    public void requestEnter() {
        // Implementujte zadost procesu 'owner' o pristup
        // ke sdilenemu zdroji 'criticalSectionName'
        this.owner.increaseTime();
        this.state = AcquisitionState.WANTED;
        this.timeForRequest = this.owner.getTime();
        for(String p : this.allAccessingProcesses) {
            if (p.equals(this.owner.id)) {
                continue;
            }
            this.owner.send(p, new Request(this.criticalSectionName));
            this.successes.put(p, false);
        }
        //throw new RuntimeException("Not implemented yet");
    }

    public void exit() {
        // Implementuje uvolneni zdroje, aby k nemu meli pristup i
        // ostatni procesy z 'allAccessingProcesses', ktere ke zdroji
        // mohou chtit pristupovat
        this.owner.increaseTime();
        this.state = AcquisitionState.RELEASED;
        this.owner.increaseTime();
        for (Request request : this.requestList) {
            this.owner.send(request.sender, new Success(this.criticalSectionName));
        }
        this.requestList.clear();

        //throw new RuntimeException("Not implemented yet");
    }

    public String getName() {
        return criticalSectionName;
    }

    public boolean isHeld() {
        return state == AcquisitionState.HELD;
    }
}
