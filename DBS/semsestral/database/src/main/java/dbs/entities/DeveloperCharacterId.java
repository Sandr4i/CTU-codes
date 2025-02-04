package dbs.entities;

import jakarta.persistence.Column;
import jakarta.persistence.Embeddable;
import org.hibernate.Hibernate;

import java.util.Objects;

@Embeddable
public class DeveloperCharacterId implements java.io.Serializable {
    private static final long serialVersionUID = 8208779844378732055L;
    @Column(name = "developer_id", nullable = false)
    private Integer developerId;

    @Column(name = "character_id", nullable = false)
    private Integer characterId;

    public Integer getDeveloperId() {
        return developerId;
    }

    public void setDeveloperId(Integer developerId) {
        this.developerId = developerId;
    }

    public Integer getCharacterId() {
        return characterId;
    }

    public void setCharacterId(Integer characterId) {
        this.characterId = characterId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || Hibernate.getClass(this) != Hibernate.getClass(o)) return false;
        DeveloperCharacterId entity = (DeveloperCharacterId) o;
        return Objects.equals(this.developerId, entity.developerId) &&
                Objects.equals(this.characterId, entity.characterId);
    }

    @Override
    public int hashCode() {
        return Objects.hash(developerId, characterId);
    }

}