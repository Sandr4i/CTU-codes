package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "covenant_membership")
public class CovenantMembership {
    @EmbeddedId
    private CovenantMembershipId id;

    @MapsId("memberId")
    @OneToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "member_id", nullable = false)
    private Character member;

    @MapsId("covenantId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "covenant_id", nullable = false)
    private Covenant covenant;

    public CovenantMembershipId getId() {
        return id;
    }

    public void setId(CovenantMembershipId id) {
        this.id = id;
    }

    public Character getMember() {
        return member;
    }

    public void setMember(Character member) {
        this.member = member;
    }

    public Covenant getCovenant() {
        return covenant;
    }

    public void setCovenant(Covenant covenant) {
        this.covenant = covenant;
    }

}