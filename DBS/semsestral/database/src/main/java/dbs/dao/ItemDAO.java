package dbs.dao;

import dbs.entities.Item;
import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class ItemDAO extends BaseDao<Item> {

    public ItemDAO() {
        super();
    }

    @Override
    public Item getById(int id) {
        TypedQuery<Item> query = em.createQuery("SELECT u FROM Item u WHERE u.id =:id", Item.class);
        query.setParameter("id", id);
        List<Item> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    //character
    public List<Item> getByCharacter(Character character) {
        TypedQuery<Item> query = em.createQuery("SELECT u FROM Item u WHERE u.character =:character", Item.class);
        query.setParameter("character", character);
        List<Item> users = query.getResultList();
        return users.isEmpty() ? null : users;
    }

    //itemName
    public List<Item> getByName(String itemName) {
        TypedQuery<Item> query = em.createQuery("SELECT u FROM Item u WHERE u.itemName =:itemName", Item.class);
        query.setParameter("itemName", itemName);
        List<Item> users = query.getResultList();
        return users.isEmpty() ? null : users;
    }

    //itemType
    public List<Item> getByType(String itemType) {
        TypedQuery<Item> query = em.createQuery("SELECT u FROM Item u WHERE u.itemType =:itemType", Item.class);
        query.setParameter("itemType", itemType);
        List<Item> users = query.getResultList();
        return users.isEmpty() ? null : users;
    }

    @Override
    public List<Item> getAll(int pageSize, int pageNumber) {
        TypedQuery<Item> query = em.createQuery("SELECT u FROM Item u ", Item.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
