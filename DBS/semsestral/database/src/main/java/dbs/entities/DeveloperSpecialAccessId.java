package dbs.entities;

import jakarta.persistence.Column;
import jakarta.persistence.Embeddable;
import org.hibernate.Hibernate;

import java.util.Objects;

@Embeddable
public class DeveloperSpecialAccessId implements java.io.Serializable {
    private static final long serialVersionUID = 8823113283338807220L;
    @Column(name = "developer_id", nullable = false)
    private Integer developerId;

    @Column(name = "special_access", nullable = false, length = 126)
    private String specialAccess;

    public Integer getDeveloperId() {
        return developerId;
    }

    public void setDeveloperId(Integer developerId) {
        this.developerId = developerId;
    }

    public String getSpecialAccess() {
        return specialAccess;
    }

    public void setSpecialAccess(String specialAccess) {
        this.specialAccess = specialAccess;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || Hibernate.getClass(this) != Hibernate.getClass(o)) return false;
        DeveloperSpecialAccessId entity = (DeveloperSpecialAccessId) o;
        return Objects.equals(this.developerId, entity.developerId) &&
                Objects.equals(this.specialAccess, entity.specialAccess);
    }

    @Override
    public int hashCode() {
        return Objects.hash(developerId, specialAccess);
    }

}