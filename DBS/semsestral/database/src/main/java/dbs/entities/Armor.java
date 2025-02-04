package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "armor")
public class Armor {
    @Id
    @Column(name = "item_id", nullable = false)
    private Integer id;

    @MapsId
    @OneToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "item_id", nullable = false)
    private Item item;

    @Column(name = "defense", nullable = false)
    private Integer defense;

    @Column(name = "type", nullable = false, length = 20)
    private String type;

    @Column(name = "passive_skill", length = 50)
    private String passiveSkill;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Item getItem() {
        return item;
    }

    public void setItem(Item item) {
        this.item = item;
    }

    public Integer getDefense() {
        return defense;
    }

    public void setDefense(Integer defense) {
        this.defense = defense;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getPassiveSkill() {
        return passiveSkill;
    }

    public void setPassiveSkill(String passiveSkill) {
        this.passiveSkill = passiveSkill;
    }

}