package dbs.dao;

import dbs.entities.Developer;
import dbs.entities.DeveloperCharacter;
import dbs.entities.Character;
import dbs.entities.SpecialAccess;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class DeveloperCharacterDAO extends BaseDao<DeveloperCharacter>{

    public DeveloperCharacterDAO() {
        super();
    }

    @Override
    public DeveloperCharacter getById(int id) {
        TypedQuery<DeveloperCharacter> query = em.createQuery("SELECT u FROM DeveloperCharacter u WHERE u.id =:id", DeveloperCharacter.class);
        query.setParameter("id", id);
        List<DeveloperCharacter> relations = query.getResultList();
        return relations.isEmpty() ? null : relations.get(0);
    }

    //developer
    public DeveloperCharacter getByDeveloper(Developer developer) {
        TypedQuery<DeveloperCharacter> query = em.createQuery("SELECT p FROM DeveloperCharacter p WHERE p.developer =:developer", DeveloperCharacter.class);
        query.setParameter("developer", developer);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    //character
    public DeveloperCharacter getByCharacter(Character character) {
        TypedQuery<DeveloperCharacter> query = em.createQuery("SELECT p FROM DeveloperCharacter p WHERE p.character =:character", DeveloperCharacter.class);
        query.setParameter("character", character);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    //specialAccess
    public DeveloperCharacter getBySpecialAccess(SpecialAccess specialAccess) {
        TypedQuery<DeveloperCharacter> query = em.createQuery("SELECT p FROM DeveloperCharacter p WHERE p.specialAccess =:specialAccess", DeveloperCharacter.class);
        query.setParameter("specialAccess", specialAccess);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    @Override
    public List<DeveloperCharacter> getAll(int pageSize, int pageNumber) {
        TypedQuery<DeveloperCharacter> query = em.createQuery("SELECT u FROM DeveloperCharacter u ", DeveloperCharacter.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
