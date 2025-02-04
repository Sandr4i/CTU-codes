package dbs.entities;

import jakarta.persistence.Column;
import jakarta.persistence.Embeddable;
import org.hibernate.Hibernate;

import java.util.Objects;

@Embeddable
public class CharacterSkillId implements java.io.Serializable {
    private static final long serialVersionUID = -1097800986861849383L;
    @Column(name = "character_id", nullable = false)
    private Integer characterId;

    @Column(name = "skill", nullable = false, length = 50)
    private String skill;

    public Integer getCharacterId() {
        return characterId;
    }

    public void setCharacterId(Integer characterId) {
        this.characterId = characterId;
    }

    public String getSkill() {
        return skill;
    }

    public void setSkill(String skill) {
        this.skill = skill;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || Hibernate.getClass(this) != Hibernate.getClass(o)) return false;
        CharacterSkillId entity = (CharacterSkillId) o;
        return Objects.equals(this.skill, entity.skill) &&
                Objects.equals(this.characterId, entity.characterId);
    }

    @Override
    public int hashCode() {
        return Objects.hash(skill, characterId);
    }

}