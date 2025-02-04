package dbs.dao;

import dbs.entities.User;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class UserDAO extends BaseDao<User>{

    public UserDAO() {
        super();
    }

    @Override
    public User getById(int id) {
        TypedQuery<User> query = em.createQuery("SELECT u FROM User u WHERE u.id =:id", User.class);
        query.setParameter("id", id);
        List<User> users = query.getResultList();
        return users.isEmpty() ? null : users.get(0);
    }

    public User getByUsername(String username) {
        TypedQuery<User> query = em.createQuery("SELECT u FROM User u WHERE u.username = :username", User.class);
        query.setParameter("username", username);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    public User getByLogin(String login) {
        TypedQuery<User> query = em.createQuery("SELECT u FROM User u WHERE u.login = :login", User.class);
        query.setParameter("login", login);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();

    }

    public User getByPassword(String password) {
        TypedQuery<User> query = em.createQuery("SELECT u FROM User u WHERE u.password = :password", User.class);
        query.setParameter("password", password);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    @Override
    public List<User> getAll(int pageSize, int pageNumber) {
        TypedQuery<User> query = em.createQuery("SELECT u FROM User u ", User.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
