package dbs.dao;

import dbs.entities.Covenant;
import dbs.entities.Character;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class CovenantDAO extends BaseDao{

    public CovenantDAO() {
        super();
    }

    @Override
    public Covenant getById(int id) {
        TypedQuery<Covenant> query = em.createQuery("SELECT p FROM Covenant p WHERE p.id =:id", Covenant.class);
        query.setParameter("id", id);
        List<Covenant> covenants = query.getResultList();
        return covenants.isEmpty() ? null : covenants.get(0);
    }

    //name
    public Covenant getByName(String covenantName) {
        TypedQuery<Covenant> query = em.createQuery("SELECT p FROM Covenant p WHERE p.covenantName =:covenantName", Covenant.class);
        query.setParameter("covenantName", covenantName);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    //location
    public Covenant getByLocation(String location) {
        TypedQuery<Covenant> query = em.createQuery("SELECT p FROM Covenant p WHERE p.location =:location", Covenant.class);
        query.setParameter("location", location);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    //banner
    public Covenant getByBanner(String banner) {
        TypedQuery<Covenant> query = em.createQuery("SELECT p FROM Covenant p WHERE p.banner =:banner", Covenant.class);
        query.setParameter("banner", banner);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    //head
    public Covenant getByHead(Character covenantHead) {
        TypedQuery<Covenant> query = em.createQuery("SELECT p FROM Covenant p WHERE p.covenantHead =:covenantHead", Covenant.class);
        query.setParameter("covenantHead", covenantHead);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    @Override
    public List<Covenant> getAll(int pageSize, int pageNumber) {
        TypedQuery<Covenant> query = em.createQuery("SELECT a FROM Covenant a ", Covenant.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
