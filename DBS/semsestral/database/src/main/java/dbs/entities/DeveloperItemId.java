package dbs.entities;

import jakarta.persistence.Column;
import jakarta.persistence.Embeddable;
import org.hibernate.Hibernate;

import java.util.Objects;

@Embeddable
public class DeveloperItemId implements java.io.Serializable {
    private static final long serialVersionUID = 3633563013835562280L;
    @Column(name = "developer_id", nullable = false)
    private Integer developerId;

    @Column(name = "item_id", nullable = false)
    private Integer itemId;

    public Integer getDeveloperId() {
        return developerId;
    }

    public void setDeveloperId(Integer developerId) {
        this.developerId = developerId;
    }

    public Integer getItemId() {
        return itemId;
    }

    public void setItemId(Integer itemId) {
        this.itemId = itemId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || Hibernate.getClass(this) != Hibernate.getClass(o)) return false;
        DeveloperItemId entity = (DeveloperItemId) o;
        return Objects.equals(this.itemId, entity.itemId) &&
                Objects.equals(this.developerId, entity.developerId);
    }

    @Override
    public int hashCode() {
        return Objects.hash(itemId, developerId);
    }

}