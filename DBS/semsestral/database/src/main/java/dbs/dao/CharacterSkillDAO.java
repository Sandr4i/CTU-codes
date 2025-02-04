package dbs.dao;

import dbs.entities.Armor;
import dbs.entities.CharacterSkill;
import dbs.entities.Skill;
import dbs.entities.Character;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.Persistence;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class CharacterSkillDAO extends BaseDao<CharacterSkill>{

    public CharacterSkillDAO() {
        super();
    }

    @Override
    public CharacterSkill getById(int id) {
        CharacterSkill characterSkill = null;
        try{
            this.em.getTransaction().begin();
            characterSkill = this.em.find(CharacterSkill.class, id);
            this.em.getTransaction().commit();
        }
        catch(Exception e){
            this.em.close();
            e.printStackTrace();
        }
        return characterSkill;
    }

    public List<CharacterSkill> getByCharacter(Character character) {
        TypedQuery<CharacterSkill> query = em.createQuery("SELECT cs FROM CharacterSkill cs WHERE cs.character = :character", CharacterSkill.class);
        query.setParameter("character", character);
        List<CharacterSkill> characterSkills = query.getResultList();
        return characterSkills;
    }

    public CharacterSkill getBySkill(Skill skill) {
        TypedQuery<CharacterSkill> query = em.createQuery("SELECT cs FROM CharacterSkill cs WHERE cs.skill = :skill", CharacterSkill.class);
        query.setParameter("skill", skill);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    @Override
    public List<CharacterSkill> getAll(int pageSize, int pageNumber) {
        TypedQuery<CharacterSkill> query = em.createQuery("SELECT a FROM CharacterSkill a ", CharacterSkill.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
