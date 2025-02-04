package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "developer_covenant")
public class DeveloperCovenant {
    @EmbeddedId
    private DeveloperCovenantId id;

    @MapsId("developerId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "developer_id", nullable = false)
    private Developer developer;

    @MapsId("covenantId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "covenant_id", nullable = false)
    private Covenant covenant;

    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "special_access", nullable = false)
    private SpecialAccess specialAccess;

    public DeveloperCovenantId getId() {
        return id;
    }

    public void setId(DeveloperCovenantId id) {
        this.id = id;
    }

    public Developer getDeveloper() {
        return developer;
    }

    public void setDeveloper(Developer developer) {
        this.developer = developer;
    }

    public Covenant getCovenant() {
        return covenant;
    }

    public void setCovenant(Covenant covenant) {
        this.covenant = covenant;
    }

    public SpecialAccess getSpecialAccess() {
        return specialAccess;
    }

    public void setSpecialAccess(SpecialAccess specialAccess) {
        this.specialAccess = specialAccess;
    }

}