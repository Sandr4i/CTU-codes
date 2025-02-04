package dbs.entities;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import org.hibernate.annotations.ColumnDefault;

import java.time.Instant;

@Entity
@Table(name = "fight")
public class Fight {
    @Id
    @ColumnDefault("nextval('fight_fight_id_seq'::regclass)")
    @Column(name = "fight_id", nullable = false)
    private Integer id;

    @Column(name = "enemy", nullable = false, length = 30)
    private String enemy;

    @Column(name = "place", nullable = false, length = 30)
    private String place;

    @Column(name = "\"time\"", nullable = false)
    private Instant time;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getEnemy() {
        return enemy;
    }

    public void setEnemy(String enemy) {
        this.enemy = enemy;
    }

    public String getPlace() {
        return place;
    }

    public void setPlace(String place) {
        this.place = place;
    }

    public Instant getTime() {
        return time;
    }

    public void setTime(Instant time) {
        this.time = time;
    }

}