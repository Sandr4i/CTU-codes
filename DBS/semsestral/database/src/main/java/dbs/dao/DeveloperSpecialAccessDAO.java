package dbs.dao;

import dbs.entities.DeveloperSpecialAccess;
import dbs.entities.Developer;
import dbs.entities.SpecialAccess;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class DeveloperSpecialAccessDAO extends BaseDao<DeveloperSpecialAccess> {

    public DeveloperSpecialAccessDAO() {
        super();
    }

    @Override
    public DeveloperSpecialAccess getById(int id) {
        TypedQuery<DeveloperSpecialAccess> query = em.createQuery("SELECT u FROM DeveloperSpecialAccess u WHERE u.id =:id", DeveloperSpecialAccess.class);
        query.setParameter("id", id);
        List<DeveloperSpecialAccess> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    //developer
    public DeveloperSpecialAccess getByDeveloper(Developer developer) {
        TypedQuery<DeveloperSpecialAccess> query = em.createQuery("SELECT p FROM DeveloperSpecialAccess p WHERE p.developer =:developer", DeveloperSpecialAccess.class);
        query.setParameter("developer", developer);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    //special access
    public DeveloperSpecialAccess getBySpecialAccess(SpecialAccess specialAccess) {
        TypedQuery<DeveloperSpecialAccess> query = em.createQuery("SELECT p FROM DeveloperSpecialAccess p WHERE p.specialAccess =:specialAccess", DeveloperSpecialAccess.class);
        query.setParameter("specialAccess", specialAccess);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    @Override
    public List<DeveloperSpecialAccess> getAll(int pageSize, int pageNumber) {
        TypedQuery<DeveloperSpecialAccess> query = em.createQuery("SELECT u FROM DeveloperSpecialAccess u ", DeveloperSpecialAccess.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
