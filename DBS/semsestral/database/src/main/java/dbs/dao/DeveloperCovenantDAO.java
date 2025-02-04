package dbs.dao;

import dbs.entities.Developer;
import dbs.entities.DeveloperCovenant;
import dbs.entities.Covenant;
import dbs.entities.SpecialAccess;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class DeveloperCovenantDAO extends BaseDao<DeveloperCovenant>{

    public DeveloperCovenantDAO() {
        super();
    }

    @Override
    public DeveloperCovenant getById(int id) {
        TypedQuery<DeveloperCovenant> query = em.createQuery("SELECT u FROM DeveloperCovenant u WHERE u.id =:id", DeveloperCovenant.class);
        query.setParameter("id", id);
        List<DeveloperCovenant> relations = query.getResultList();
        return relations.isEmpty() ? null : relations.get(0);
    }

    //developer
    public DeveloperCovenant getByDeveloper(Developer developer) {
        TypedQuery<DeveloperCovenant> query = em.createQuery("SELECT p FROM DeveloperCovenant p WHERE p.developer =:developer", DeveloperCovenant.class);
        query.setParameter("developer", developer);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    //character
    public DeveloperCovenant getByCharacter(Covenant covenant) {
        TypedQuery<DeveloperCovenant> query = em.createQuery("SELECT p FROM DeveloperCovenant p WHERE p.covenant =:covenant", DeveloperCovenant.class);
        query.setParameter("covenant", covenant);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    //specialAccess
    public DeveloperCovenant getBySpecialAccess(SpecialAccess specialAccess) {
        TypedQuery<DeveloperCovenant> query = em.createQuery("SELECT p FROM DeveloperCovenant p WHERE p.specialAccess =:specialAccess", DeveloperCovenant.class);
        query.setParameter("specialAccess", specialAccess);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    @Override
    public List<DeveloperCovenant> getAll(int pageSize, int pageNumber) {
        TypedQuery<DeveloperCovenant> query = em.createQuery("SELECT u FROM DeveloperCovenant u ", DeveloperCovenant.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
