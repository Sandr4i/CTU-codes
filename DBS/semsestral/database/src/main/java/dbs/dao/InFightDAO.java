package dbs.dao;

import dbs.entities.Fight;
import dbs.entities.InFight;
import dbs.entities.Character;
import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class InFightDAO extends BaseDao<InFight> {

    public InFightDAO() {
        super();
    }

    @Override
    public InFight getById(int id) {
        TypedQuery<InFight> query = em.createQuery("SELECT u FROM InFight u WHERE u.id =:id", InFight.class);
        query.setParameter("id", id);
        List<InFight> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    //character
    public List<InFight> getByCharacter(Character character) {
        TypedQuery<InFight> query = em.createQuery("SELECT u FROM InFight u WHERE u.character =:character", InFight.class);
        query.setParameter("character", character);
        List<InFight> fights = query.getResultList();
        return fights.isEmpty() ? null : fights;
    }

    //fight
    public List<InFight> getByFight(Fight fight) {
        TypedQuery<InFight> query = em.createQuery("SELECT u FROM InFight u WHERE u.fight =:fight", InFight.class);
        query.setParameter("fight", fight);
        List<InFight> fights = query.getResultList();
        return fights.isEmpty() ? null : fights;
    }


    @Override
    public List<InFight> getAll(int pageSize, int pageNumber) {
        TypedQuery<InFight> query = em.createQuery("SELECT u FROM InFight u ", InFight.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
