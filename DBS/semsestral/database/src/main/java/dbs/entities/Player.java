package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "player")
@PrimaryKeyJoinColumn(name = "user_id")
public class Player extends User{

}