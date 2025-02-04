package dbs.dao;

import dbs.entities.Weapon;

import jakarta.persistence.TypedQuery;

import java.util.List;

public class WeaponDAO extends BaseDao<Weapon>{

    public WeaponDAO() {
        super();
    }

    @Override
    public Weapon getById(int id) {
        TypedQuery<Weapon> query = em.createQuery("SELECT a FROM Weapon a JOIN Item i ON i.id = a.id WHERE a.id = :id", Weapon.class);
        query.setParameter("id", id);
        List<Weapon> list = query.getResultList();
        return list.isEmpty() ? null : list.get(0);
    }

    public Weapon getByDamage(Integer damage) {
        TypedQuery<Weapon> query = em.createQuery("SELECT a FROM Weapon a WHERE a.damage = :damage", Weapon.class);
        query.setParameter("damage", damage);
        List<Weapon> list = query.getResultList();
        return list.isEmpty() ? null : list.get(0);
    }

    @Override
    public List<Weapon> getAll(int pageSize, int pageNumber) {
        TypedQuery<Weapon> query = em.createQuery("SELECT a FROM Weapon a ", Weapon.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
