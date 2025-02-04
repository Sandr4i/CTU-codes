package dbs.dao;

import dbs.entities.SpecialAccess;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class SpecialAccessDAO extends BaseDao<SpecialAccess> {

    public SpecialAccessDAO() {
        super();
    }

    @Override
    public SpecialAccess getById(int id) {
        TypedQuery<SpecialAccess> query = em.createQuery("SELECT u FROM SpecialAccess u WHERE u.id =:id", SpecialAccess.class);
        query.setParameter("id", id);
        List<SpecialAccess> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    public SpecialAccess getByName(String name) {
        TypedQuery<SpecialAccess> query = em.createQuery("SELECT u FROM SpecialAccess u WHERE u.name = :name", SpecialAccess.class);
        query.setParameter("name", name);
        List<SpecialAccess> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    @Override
    public List<SpecialAccess> getAll(int pageSize, int pageNumber) {
        TypedQuery<SpecialAccess> query = em.createQuery("SELECT u FROM SpecialAccess u ", SpecialAccess.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
