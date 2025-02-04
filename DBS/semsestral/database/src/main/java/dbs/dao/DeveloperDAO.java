package dbs.dao;

import dbs.entities.Developer;
import dbs.entities.User;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class DeveloperDAO extends BaseDao<Developer> {

    public DeveloperDAO() {
        super();
    }

    @Override
    public Developer getById(int id) {
        TypedQuery<Developer> query = em.createQuery("SELECT d FROM Developer d JOIN User u ON u.id = d.id WHERE d.id =:id", Developer.class);
        query.setParameter("id", id);
        List<Developer> developers = query.getResultList();
        return developers.isEmpty() ? null : developers.get(0);
    }

    //user
    public Developer getByUsername(String username) {
        TypedQuery<Developer> query = em.createQuery("SELECT d FROM Developer d JOIN User u ON u.id = d.id WHERE d.username =:username", Developer.class);
        query.setParameter("username", username);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    @Override
    public List<Developer> getAll(int pageSize, int pageNumber) {
        TypedQuery<Developer> query = em.createQuery("SELECT a FROM Developer a ", Developer.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
