package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "hostile")
public class Hostile {
    @EmbeddedId
    private HostileId id;

    @MapsId("covenantId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "covenant_id", nullable = false)
    private Covenant covenant;

    @MapsId("hostileId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "hostile_id", nullable = false)
    private Covenant hostile;

    public HostileId getId() {
        return id;
    }

    public void setId(HostileId id) {
        this.id = id;
    }

    public Covenant getCovenant() {
        return covenant;
    }

    public void setCovenant(Covenant covenant) {
        this.covenant = covenant;
    }

    public Covenant getHostile() {
        return hostile;
    }

    public void setHostile(Covenant hostile) {
        this.hostile = hostile;
    }

}