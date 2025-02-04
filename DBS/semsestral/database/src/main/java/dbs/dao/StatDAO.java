package dbs.dao;

import dbs.entities.Stat;
import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class StatDAO extends BaseDao<Stat> {

    public StatDAO() {
        super();
    }

    @Override
    public Stat getById(int id) {
        TypedQuery<Stat> query = em.createQuery("SELECT u FROM Stat u WHERE u.id =:id", Stat.class);
        query.setParameter("id", id);
        List<Stat> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    public Stat getByName(String name) {
        TypedQuery<Stat> query = em.createQuery("SELECT u FROM Stat u WHERE u.name =:name", Stat.class);
        query.setParameter("name", name);
        List<Stat> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    @Override
    public List<Stat> getAll(int pageSize, int pageNumber) {
        TypedQuery<Stat> query = em.createQuery("SELECT u FROM Stat u ", Stat.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
