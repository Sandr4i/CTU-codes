package dbs.dao;

import dbs.entities.Character;

import dbs.entities.User;
import jakarta.persistence.TypedQuery;

import java.time.LocalDate;
import java.util.List;

public class CharacterDAO extends BaseDao<Character> {

    public CharacterDAO() {
        super();
    }

    @Override
    public Character getById(int id) {
        TypedQuery<Character> query = em.createQuery("SELECT c FROM Character c WHERE c.id = :id", Character.class);
        query.setParameter("id", id);
        List<Character> characters = query.getResultList();
        return characters.isEmpty() ? null : characters.get(0);
    }

    public Character getByName(String name) {
        TypedQuery<Character> query = em.createQuery("SELECT c FROM Character c WHERE c.name = :name", Character.class);
        query.setParameter("name", name);
        List<Character> characters = query.getResultList();
        return characters.isEmpty() ? null : characters.get(0);
    }

    public Character getByUser(User user) {
        TypedQuery<Character> query = em.createQuery("SELECT c FROM Character c WHERE c.user = :user", Character.class);
        query.setParameter("user", user);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    public Character getByCreationDate(LocalDate creationDate){
        TypedQuery<Character> query = em.createQuery("SELECT c FROM Character c WHERE c.creationDate = :creationDate", Character.class);
        query.setParameter("creationDate", creationDate);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    @Override
    public List<Character> getAll(int pageSize, int pageNumber) {
        TypedQuery<Character> query = em.createQuery("SELECT a FROM Character a ", Character.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }

}
