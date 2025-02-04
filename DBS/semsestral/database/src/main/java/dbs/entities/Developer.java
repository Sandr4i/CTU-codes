package dbs.entities;

import jakarta.persistence.*;

@Entity
@Table(name = "developer")
@PrimaryKeyJoinColumn(name = "user_id")
public class Developer extends User{
//    @Id
//    @Column(name = "user_id", nullable = false)
//    private Integer id;
//
////    @MapsId
////    @OneToOne(fetch = FetchType.LAZY, optional = false)
////    @JoinColumn(name = "user_id", nullable = false)
////    private User user;
//
//    public Integer getId() {
//        return id;
//    }
//
//    public void setId(Integer id) {
//        this.id = id;
//    }

//    public User getUser() {
//        return user;
//    }
//
//    public void setUser(User user) {
//        this.user = user;
//    }

}