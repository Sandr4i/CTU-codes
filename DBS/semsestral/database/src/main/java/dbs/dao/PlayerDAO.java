package dbs.dao;

import dbs.entities.Player;
import dbs.entities.User;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class PlayerDAO extends BaseDao<Player> {

    public PlayerDAO() {
        super();
    }

    @Override
    public Player getById(int id) {
        TypedQuery<Player> query = em.createQuery("SELECT p FROM Player p JOIN User u ON u.id = p.id WHERE p.id =:id", Player.class);
        query.setParameter("id", id);
        List<Player> players = query.getResultList();
        return players.isEmpty() ? null : players.get(0);
    }

    public Player getByUsername(String username) {
        TypedQuery<Player> query = em.createQuery("SELECT p FROM Player p JOIN User u ON u.id = p.id WHERE p.username =:username", Player.class);
        query.setParameter("username", username);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    @Override
    public List<Player> getAll(int pageSize, int pageNumber) {
        TypedQuery<Player> query = em.createQuery("SELECT a FROM Player a ", Player.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
