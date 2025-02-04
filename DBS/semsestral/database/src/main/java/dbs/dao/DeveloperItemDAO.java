package dbs.dao;

import dbs.entities.DeveloperItem;
import dbs.entities.Item;
import dbs.entities.SpecialAccess;
import dbs.entities.Developer;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class DeveloperItemDAO extends BaseDao<DeveloperItem>{

    public DeveloperItemDAO() {
        super();
    }

    @Override
    public DeveloperItem getById(int id) {
        TypedQuery<DeveloperItem> query = em.createQuery("SELECT u FROM DeveloperItem u WHERE u.id =:id", DeveloperItem.class);
        query.setParameter("id", id);
        List<DeveloperItem> relations = query.getResultList();
        return relations.isEmpty() ? null : relations.get(0);
    }

    //developer
    public DeveloperItem getByDeveloper(Developer developer) {
        TypedQuery<DeveloperItem> query = em.createQuery("SELECT p FROM DeveloperItem p WHERE p.developer =:developer", DeveloperItem.class);
        query.setParameter("developer", developer);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    //character
    public DeveloperItem getByItem(Item item) {
        TypedQuery<DeveloperItem> query = em.createQuery("SELECT p FROM DeveloperItem p WHERE p.item =:item", DeveloperItem.class);
        query.setParameter("item", item);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    //specialAccess
    public DeveloperItem getBySpecialAccess(SpecialAccess specialAccess) {
        TypedQuery<DeveloperItem> query = em.createQuery("SELECT p FROM DeveloperItem p WHERE p.specialAccess =:specialAccess", DeveloperItem.class);
        query.setParameter("specialAccess", specialAccess);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    @Override
    public List<DeveloperItem> getAll(int pageSize, int pageNumber) {
        TypedQuery<DeveloperItem> query = em.createQuery("SELECT u FROM DeveloperItem u ", DeveloperItem.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
