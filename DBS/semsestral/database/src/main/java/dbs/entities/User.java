package dbs.entities;

import jakarta.persistence.*;
import org.hibernate.annotations.ColumnDefault;

@Entity
@Table(name = "\"User\"")
@Inheritance(strategy = InheritanceType.JOINED)
public class User {
    @Id
    @ColumnDefault("nextval('\"User_user_id_seq\"'::regclass)")
    @Column(name = "user_id", nullable = false)
    private Integer id;

    @Column(name = "username", nullable = false, length = 15)
    private String username;

    @Column(name = "login", nullable = false, length = 15)
    private String login;

    @Column(name = "password", nullable = false, length = 20)
    private String password;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getLogin() {
        return login;
    }

    public void setLogin(String login) {
        this.login = login;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

}