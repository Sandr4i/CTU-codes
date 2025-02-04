package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "character_skills")
public class CharacterSkill {
    @EmbeddedId
    private CharacterSkillId id;

    @MapsId("characterId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "character_id", nullable = false)
    private Character character;

    @MapsId("skill")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "skill", nullable = false)
    private Skill skill;

    public CharacterSkillId getId() {
        return id;
    }

    public void setId(CharacterSkillId id) {
        this.id = id;
    }

    public Character getCharacter() {
        return character;
    }

    public void setCharacter(Character character) {
        this.character = character;
    }

    public Skill getSkill() {
        return skill;
    }

    public void setSkill(Skill skill) {
        this.skill = skill;
    }

}