package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "in_fight")
public class InFight {
    @EmbeddedId
    private InFightId id;

    @MapsId("characterId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "character_id", nullable = false)
    private Character character;

    @MapsId("fightId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "fight_id", nullable = false)
    private Fight fight;

    public InFightId getId() {
        return id;
    }

    public void setId(InFightId id) {
        this.id = id;
    }

    public Character getCharacter() {
        return character;
    }

    public void setCharacter(Character character) {
        this.character = character;
    }

    public Fight getFight() {
        return fight;
    }

    public void setFight(Fight fight) {
        this.fight = fight;
    }

}