package dbs.dao;

import dbs.entities.Covenant;
import dbs.entities.Hostile;

import dbs.entities.HostileId;
import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class HostileDAO extends BaseDao<Hostile> {

    public HostileDAO() {
        super();
    }

    @Override
    public Hostile getById(int id) {
        TypedQuery<Hostile> query = em.createQuery("SELECT u FROM Hostile u WHERE u.id =:id", Hostile.class);
        query.setParameter("id", id);
        List<Hostile> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    //HostileId
    public List<Hostile> getByHostileId(HostileId id) {
        TypedQuery<Hostile> query = em.createQuery("SELECT u FROM Hostile u WHERE u.hostile.id =:id", Hostile.class);
        query.setParameter("id", id);
        List<Hostile> hostiles = query.getResultList();
        return hostiles.isEmpty() ? null : hostiles;
    }

    //covenant
    public Hostile getByCovenant(Covenant covenant) {
        TypedQuery<Hostile> query = em.createQuery("SELECT u FROM Hostile u WHERE u.covenant =:covenant", Hostile.class);
        query.setParameter("covenant", covenant);
        List<Hostile> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    //hostile -- hostile for the covenant
    public Hostile getByHostile(Covenant hostile) {
        TypedQuery<Hostile> query = em.createQuery("SELECT u FROM Hostile u WHERE u.hostile =:hostile", Hostile.class);
        query.setParameter("hostile", hostile);
        List<Hostile> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    @Override
    public List<Hostile> getAll(int pageSize, int pageNumber) {
        TypedQuery<Hostile> query = em.createQuery("SELECT u FROM Hostile u ", Hostile.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
