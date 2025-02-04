package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "stats")
public class Stat {
    @Id
    @Column(name = "character_id", nullable = false)
    private Integer id;

    @MapsId
    @OneToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "character_id", nullable = false)
    private Character character;

    @Column(name = "vigor", nullable = false)
    private Integer vigor;

    @Column(name = "endurance", nullable = false)
    private Integer endurance;

    @Column(name = "mind", nullable = false)
    private Integer mind;

    @Column(name = "strength", nullable = false)
    private Integer strength;

    @Column(name = "dexterity", nullable = false)
    private Integer dexterity;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Character getCharacter() {
        return character;
    }

    public void setCharacter(Character character) {
        this.character = character;
    }

    public Integer getVigor() {
        return vigor;
    }

    public void setVigor(Integer vigor) {
        this.vigor = vigor;
    }

    public Integer getEndurance() {
        return endurance;
    }

    public void setEndurance(Integer endurance) {
        this.endurance = endurance;
    }

    public Integer getMind() {
        return mind;
    }

    public void setMind(Integer mind) {
        this.mind = mind;
    }

    public Integer getStrength() {
        return strength;
    }

    public void setStrength(Integer strength) {
        this.strength = strength;
    }

    public Integer getDexterity() {
        return dexterity;
    }

    public void setDexterity(Integer dexterity) {
        this.dexterity = dexterity;
    }

}