package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "developer_special_access")
public class DeveloperSpecialAccess {
    @EmbeddedId
    private DeveloperSpecialAccessId id;

    @MapsId("developerId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "developer_id", nullable = false)
    private Developer developer;

    @MapsId("specialAccess")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "special_access", nullable = false)
    private SpecialAccess specialAccess;

    public DeveloperSpecialAccessId getId() {
        return id;
    }

    public void setId(DeveloperSpecialAccessId id) {
        this.id = id;
    }

    public Developer getDeveloper() {
        return developer;
    }

    public void setDeveloper(Developer developer) {
        this.developer = developer;
    }

    public SpecialAccess getSpecialAccess() {
        return specialAccess;
    }

    public void setSpecialAccess(SpecialAccess specialAccess) {
        this.specialAccess = specialAccess;
    }

}