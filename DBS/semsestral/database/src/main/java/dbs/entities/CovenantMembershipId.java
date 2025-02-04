package dbs.entities;

import jakarta.persistence.Column;
import jakarta.persistence.Embeddable;
import org.hibernate.Hibernate;

import java.util.Objects;

@Embeddable
public class CovenantMembershipId implements java.io.Serializable {
    private static final long serialVersionUID = -8078057441080461344L;
    @Column(name = "member_id", nullable = false)
    private Integer memberId;

    @Column(name = "covenant_id", nullable = false)
    private Integer covenantId;

    public Integer getMemberId() {
        return memberId;
    }

    public void setMemberId(Integer memberId) {
        this.memberId = memberId;
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
        CovenantMembershipId entity = (CovenantMembershipId) o;
        return Objects.equals(this.covenantId, entity.covenantId) &&
                Objects.equals(this.memberId, entity.memberId);
    }

    @Override
    public int hashCode() {
        return Objects.hash(covenantId, memberId);
    }

}