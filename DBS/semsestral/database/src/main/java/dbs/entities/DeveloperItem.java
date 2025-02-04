package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "developer_item")
public class DeveloperItem {
    @EmbeddedId
    private DeveloperItemId id;

    @MapsId("developerId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "developer_id", nullable = false)
    private Developer developer;

    @MapsId("itemId")
    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "item_id", nullable = false)
    private Item item;

    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "special_access", nullable = false)
    private SpecialAccess specialAccess;

    public DeveloperItemId getId() {
        return id;
    }

    public void setId(DeveloperItemId id) {
        this.id = id;
    }

    public Developer getDeveloper() {
        return developer;
    }

    public void setDeveloper(Developer developer) {
        this.developer = developer;
    }

    public Item getItem() {
        return item;
    }

    public void setItem(Item item) {
        this.item = item;
    }

    public SpecialAccess getSpecialAccess() {
        return specialAccess;
    }

    public void setSpecialAccess(SpecialAccess specialAccess) {
        this.specialAccess = specialAccess;
    }

}