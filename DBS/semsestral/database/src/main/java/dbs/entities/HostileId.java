package dbs.entities;

import jakarta.persistence.Column;
import jakarta.persistence.Embeddable;
import org.hibernate.Hibernate;

import java.util.Objects;

@Embeddable
public class HostileId implements java.io.Serializable {
    private static final long serialVersionUID = 2366754997975526071L;
    @Column(name = "covenant_id", nullable = false)
    private Integer covenantId;

    @Column(name = "hostile_id", nullable = false)
    private Integer hostileId;

    public Integer getCovenantId() {
        return covenantId;
    }

    public void setCovenantId(Integer covenantId) {
        this.covenantId = covenantId;
    }

    public Integer getHostileId() {
        return hostileId;
    }

    public void setHostileId(Integer hostileId) {
        this.hostileId = hostileId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || Hibernate.getClass(this) != Hibernate.getClass(o)) return false;
        HostileId entity = (HostileId) o;
        return Objects.equals(this.covenantId, entity.covenantId) &&
                Objects.equals(this.hostileId, entity.hostileId);
    }

    @Override
    public int hashCode() {
        return Objects.hash(covenantId, hostileId);
    }

}