package dbs.entities;

import jakarta.persistence.Column;
import jakarta.persistence.Embeddable;
import org.hibernate.Hibernate;

import java.util.Objects;

@Embeddable
public class DeveloperCovenantId implements java.io.Serializable {
    private static final long serialVersionUID = 7951920791610360073L;
    @Column(name = "developer_id", nullable = false)
    private Integer developerId;

    @Column(name = "covenant_id", nullable = false)
    private Integer covenantId;

    public Integer getDeveloperId() {
        return developerId;
    }

    public void setDeveloperId(Integer developerId) {
        this.developerId = developerId;
    }

    public Integer getCovenantId() {
        return covenantId;
    }

    public void setCovenantId(Integer covenantId) {
        this.covenantId = covenantId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || Hibernate.getClass(this) != Hibernate.getClass(o)) return false;
        DeveloperCovenantId entity = (DeveloperCovenantId) o;
        return Objects.equals(this.developerId, entity.developerId) &&
                Objects.equals(this.covenantId, entity.covenantId);
    }

    @Override
    public int hashCode() {
        return Objects.hash(developerId, covenantId);
    }

}