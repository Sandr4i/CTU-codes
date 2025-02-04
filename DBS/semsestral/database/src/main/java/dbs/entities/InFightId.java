package dbs.entities;

import jakarta.persistence.Column;
import jakarta.persistence.Embeddable;
import org.hibernate.Hibernate;

import java.util.Objects;

@Embeddable
public class InFightId implements java.io.Serializable {
    private static final long serialVersionUID = -6825045835375767676L;
    @Column(name = "character_id", nullable = false)
    private Integer characterId;

    @Column(name = "fight_id", nullable = false)
    private Integer fightId;

    public Integer getCharacterId() {
        return characterId;
    }

    public void setCharacterId(Integer characterId) {
        this.characterId = characterId;
    }

    public Integer getFightId() {
        return fightId;
    }

    public void setFightId(Integer fightId) {
        this.fightId = fightId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || Hibernate.getClass(this) != Hibernate.getClass(o)) return false;
        InFightId entity = (InFightId) o;
        return Objects.equals(this.characterId, entity.characterId) &&
                Objects.equals(this.fightId, entity.fightId);
    }

    @Override
    public int hashCode() {
        return Objects.hash(characterId, fightId);
    }

}