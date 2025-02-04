package dbs.dao;

import dbs.entities.Armor;
import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.Persistence;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class ArmorDAO extends BaseDao<Armor>{

    public ArmorDAO() {
        super();
    }

    @Override
    public Armor getById(int id) {
        TypedQuery<Armor> query = em.createQuery("SELECT a FROM Armor a JOIN Item i ON i.id = a.id WHERE a.id = :id", Armor.class);
        query.setParameter("id", id);
        List<Armor> list = query.getResultList();
        return list.isEmpty() ? null : list.get(0);
    }

    public Armor getByPassiveSkill(String passiveSkill) {
        TypedQuery<Armor> query = em.createQuery("SELECT a FROM Armor a WHERE a.passiveSkill = :passiveSkill", Armor.class);
        query.setParameter("passiveSkill", passiveSkill);
        List<Armor> list = query.getResultList();
        return list.isEmpty() ? null : list.get(0);
    }

    public Armor getByDefense(Integer defense) {
        TypedQuery<Armor> query = em.createQuery("SELECT a FROM Armor a WHERE a.defense = :defense", Armor.class);
        query.setParameter("defense", defense);
        List<Armor> list = query.getResultList();
        return list.isEmpty() ? null : list.get(0);
    }

    @Override
    public List<Armor> getAll(int pageSize, int pageNumber) {
        TypedQuery<Armor> query = em.createQuery("SELECT a FROM Armor a ", Armor.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
