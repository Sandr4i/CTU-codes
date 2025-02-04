package dbs.dao;

import dbs.entities.Skill;
import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class SkillDAO extends BaseDao<Skill> {

    public SkillDAO() {
        super();
    }

    @Override
    public Skill getById(int id) {
        TypedQuery<Skill> query = em.createQuery("SELECT u FROM Skill u WHERE u.id =:id", Skill.class);
        query.setParameter("id", id);
        List<Skill> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    public Skill getByName(String name) {
        TypedQuery<Skill> query = em.createQuery("SELECT u FROM Skill u WHERE u.name =:name", Skill.class);
        query.setParameter("name", name);
        List<Skill> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    @Override
    public List<Skill> getAll(int pageSize, int pageNumber) {
        TypedQuery<Skill> query = em.createQuery("SELECT u FROM Skill u ", Skill.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
