package dbs.entities;

import jakarta.persistence.*;
import org.hibernate.annotations.ColumnDefault;

@Entity
@Table(name = "covenant")
public class Covenant {
    @Id
    @ColumnDefault("nextval('covenant_covenant_id_seq'::regclass)")
    @Column(name = "covenant_id", nullable = false)
    private Integer id;

    @Column(name = "covenant_name", nullable = false, length = 50)
    private String covenantName;

    @OneToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "covenant_head", nullable = false)
    private Character covenantHead;

    @Column(name = "location", length = 50)
    private String location;

    @Column(name = "banner", length = 100)
    private String banner;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getCovenantName() {
        return covenantName;
    }

    public void setCovenantName(String covenantName) {
        this.covenantName = covenantName;
    }

    public Character getCovenantHead() {
        return covenantHead;
    }

    public void setCovenantHead(Character covenantHead) {
        this.covenantHead = covenantHead;
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }

    public String getBanner() {
        return banner;
    }

    public void setBanner(String banner) {
        this.banner = banner;
    }

}