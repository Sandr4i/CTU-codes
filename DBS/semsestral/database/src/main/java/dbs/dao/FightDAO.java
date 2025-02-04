package dbs.dao;

import dbs.entities.Fight;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.time.Instant;
import java.util.List;

public class FightDAO extends BaseDao<Fight> {

    public FightDAO() {
        super();
    }

    @Override
    public Fight getById(int id) {
        TypedQuery<Fight> query = em.createQuery("SELECT u FROM Fight u WHERE u.id =:id", Fight.class);
        query.setParameter("id", id);
        List<Fight> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    //enemy
    public List<Fight> getByEnemy(String enemy){
        TypedQuery<Fight> query = em.createQuery("SELECT f FROM Fight f WHERE f.enemy = :enemy", Fight.class);
        query.setParameter("enemy", enemy);
        List<Fight> users = query.getResultList();
        return users.isEmpty() ? null : users;
    }

    //place
    public List<Fight> getByPlace(String place){
        TypedQuery<Fight> query = em.createQuery("SELECT f FROM Fight f WHERE f.place = :place", Fight.class);
        query.setParameter("place", place);
        List<Fight> users = query.getResultList();
        return users.isEmpty() ? null : users;
    }

    //time
    public List<Fight> getByTime(Instant time){
        TypedQuery<Fight> query = em.createQuery("SELECT f FROM Fight f WHERE f.time = :time", Fight.class);
        query.setParameter("time", time);
        List<Fight> users = query.getResultList();
        return users.isEmpty() ? null : users;
    }

    @Override
    public List<Fight> getAll(int pageSize, int pageNumber) {
        TypedQuery<Fight> query = em.createQuery("SELECT u FROM Fight u ", Fight.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
