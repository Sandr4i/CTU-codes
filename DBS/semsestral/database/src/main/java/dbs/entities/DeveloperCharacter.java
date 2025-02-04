package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "developer_character")
public class DeveloperCharacter {
    @EmbeddedId
    private DeveloperCharacterId id;

    @MapsId("developerId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "developer_id", nullable = false)
    private Developer developer;

    @MapsId("characterId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "character_id", nullable = false)
    private Character character;

    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "special_access", nullable = false)
    private SpecialAccess specialAccess;

    public DeveloperCharacterId getId() {
        return id;
    }

    public void setId(DeveloperCharacterId id) {
        this.id = id;
    }

    public Developer getDeveloper() {
        return developer;
    }

    public void setDeveloper(Developer developer) {
        this.developer = developer;
    }

    public Character getCharacter() {
        return character;
    }

    public void setCharacter(Character character) {
        this.character = character;
    }

    public SpecialAccess getSpecialAccess() {
        return specialAccess;
    }

    public void setSpecialAccess(SpecialAccess specialAccess) {
        this.specialAccess = specialAccess;
    }

}