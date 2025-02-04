package dbs.dao;

import dbs.entities.Character;
import dbs.entities.Covenant;
import dbs.entities.CovenantMembership;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class CovenantMembershipDAO extends BaseDao<CovenantMembership>{

    public CovenantMembershipDAO() {
        super();
    }

    @Override
    public CovenantMembership getById(int id) {
        TypedQuery<CovenantMembership> query = em.createQuery("SELECT p FROM CovenantMembership p WHERE p.id =:id", CovenantMembership.class);
        query.setParameter("id", id);
        List<CovenantMembership> covenants = query.getResultList();
        return covenants.isEmpty() ? null : covenants.get(0);
    }

    //member
    public CovenantMembership getByMember(Character member){
        TypedQuery<CovenantMembership> query = em.createQuery("SELECT p FROM CovenantMembership p WHERE p.member =:member", CovenantMembership.class);
        query.setParameter("member", member);
        return query.getResultList().isEmpty() ? null : query.getSingleResult();
    }

    //covenant
    public List<CovenantMembership> getByCovenant(Covenant covenant){
        TypedQuery<CovenantMembership> query = em.createQuery("SELECT p FROM CovenantMembership p WHERE p.covenant =:covenant", CovenantMembership.class);
        query.setParameter("covenant", covenant);
        List<CovenantMembership> covenants = query.getResultList();
        return covenants.isEmpty() ? null : covenants;
    }

    @Override
    public List<CovenantMembership> getAll(int pageSize, int pageNumber) {
        TypedQuery<CovenantMembership> query = em.createQuery("SELECT a FROM CovenantMembership a ", CovenantMembership.class);
        query.setFirstResult((pageNumber - 1) * pageSize);
        query.setMaxResults(pageSize);
        return query.getResultList();
    }
}
