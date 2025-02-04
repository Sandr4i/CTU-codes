package dbs.dao;

import jakarta.persistence.*;

import java.util.List;

public class BaseDao<T> {

    private static final EntityManagerFactory emf = Persistence.createEntityManagerFactory("ApplicationPU");

    @PersistenceContext
    public final EntityManager em;

    public BaseDao(){
        this.em = emf.createEntityManager();
    }

    public boolean add(T entity){
        try{
            em.getTransaction().begin();
            em.persist(entity);
            em.getTransaction().commit();
        }
        catch(RollbackException e) {
            System.out.println("RollbackException: ");
            e.printStackTrace();
            return false;
        }
        catch (Exception e){
            System.out.println("Exception: ");
            e.printStackTrace();
            return false;
        }
//        em.getTransaction().begin();
//        em.persist(entity);
//        em.getTransaction().commit();
        return true;
    }

    public T getById(int id){
        return null;
    }

    public List<T> getAll(int pageSize, int pageNumber){
        return null;
    }

    public boolean update(T entity){
        boolean result = false;
        try{
            em.getTransaction().begin();
            em.merge(entity);
            em.getTransaction().commit();
            result = true;
        }
        catch(Exception e){
            em.close();
            result = false;
            e.printStackTrace();
        }
        return result;
    }

    public boolean delete(T entity){
        boolean result = false;
        try{
            em.getTransaction().begin();
            em.remove(entity);
            em.getTransaction().commit();
            result = true;
        }
        catch(Exception e){
            em.close();
            result = false;
            e.printStackTrace();
        }
        return result;
    }

    public void close(){
        if(em.isOpen()){
            em.close();
        }
    }

    public static void shutDown(){
        if(emf.isOpen()){
            emf.close();
        }
    }
}