package dbs;

import dbs.dao.*;
import dbs.entities.*;
import dbs.entities.Character;
import jakarta.persistence.EntityTransaction;

import java.time.LocalDate;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        test1();
        System.out.println("-----------------------");
        test2();
        System.out.println("-----------------------");
        test3();
        System.out.println("-----------------------");
        test4();
        System.out.println("-----------------------");

        clearData();
        BaseDao.shutDown();
    }

    public static void test1(){
        System.out.println("TEST: Create a player");
        PlayerDAO playerDAO = new PlayerDAO();
        Player player = new Player();
        //player.setId(123);
//        UserDAO userDAO = new UserDAO();
//        User user = new User();
//        user.setUsername("ExampleUserName");
//        user.setLogin("ExampleLogin");
//        user.setPassword("ExamplePassword");
//        user.setId(123);
        player.setId(123);
        player.setLogin("Loginexamople");
        player.setPassword("passwordexample");
        player.setUsername("username");

        //player.setUser(user);
        //player.setId(123);

//        System.out.print("Check if user was created: ");
//        if(userDAO.add(user)){
//            System.out.print("User was created\n");
//        }
//        else {
//            System.out.print("ERROR: Couldn't create user\n");
//        }

        System.out.print("Check if player was created: ");
        if(playerDAO.add(player)){
            System.out.print("Player was created\n");
        }
        else {
            System.out.print("ERROR: Couldn't create player\n");
        }



        playerDAO.close();
        //userDAO.close();
    }

    public static void test2(){
        System.out.println("TEST2: Create a developer");
        DeveloperDAO developerDAO = new DeveloperDAO();
        Developer developer = new Developer();
        developer.setId(456);
        developer.setLogin("Loginexamople2");
        developer.setPassword("passwordexample2");
        developer.setUsername("username2");

        System.out.print("Check if developer was created: ");
        if(developerDAO.add(developer)){
            System.out.print("Developer was created\n");
        }
        else {
            System.out.print("ERROR: Couldn't create developer\n");
        }

        developerDAO.close();
    }

    public static void test3(){
        System.out.println("TEST3: Create a character");
        PlayerDAO playerDAO = new PlayerDAO();
        Player player = new Player();

        CharacterDAO characterDAO = new CharacterDAO();
        Character character = new Character();

        player.setUsername("user3");
        player.setLogin("ExampleLogin3");
        player.setPassword("ExamplePassword3");
        player.setId(789);

        character.setUser(player);
        character.setId(0000);
        character.setName("name");
        character.setCreationDate(LocalDate.of(2005, 5, 5));

        if(playerDAO.add(player)){
            System.out.print("User was created\n");
        }
        else {
            System.out.print("ERROR: Couldn't create user\n");
        }

        System.out.print("Check if character was created: ");
        if(characterDAO.add(character)){
            System.out.print("Character was created\n");
        }
        else {
            System.out.print("ERROR: Couldn't create character\n");
        }

        characterDAO.close();
        playerDAO.close();
    }

    public static void test4(){
        System.out.println("TEST4: Transaction");
        CharacterDAO characterDAO = new CharacterDAO();
        StatDAO statDAO = new StatDAO();
        PlayerDAO playerDAO = new PlayerDAO();

        EntityTransaction transaction = characterDAO.em.getTransaction();
        try{
            transaction.begin();
            List<Stat> stats = statDAO.em.createQuery("SELECT s FROM Stat s WHERE s.endurance < 15", Stat.class).getResultList();
            if (stats.isEmpty()) {
                throw new Exception("No state found");
            }

            Stat selected = stats.get(0);
            int originalValue = selected.getEndurance();
            selected.setEndurance(originalValue - 5);

            statDAO.update(selected);
            transaction.commit();

            Stat updated = statDAO.getById(selected.getId());
            if (updated == null) {
                throw new Exception("No updated stat found");
            }
            if(updated.getEndurance() != originalValue - 5){
                throw new Exception("Endurance was not updated correctly");
            }

            System.out.println("Transaction ended successfully");
        } catch (Exception e) {
            if (transaction.isActive()){
                transaction.rollback();
            }
            throw new RuntimeException(e);
        }
    }

    public static void clearData(){
        System.out.println("Clearing data...");

        CharacterDAO characterDAO = new CharacterDAO();
        Character character = characterDAO.getById(0000);
        if(character == null){
            System.out.println("No character found");
        }
        else {
            characterDAO.delete(character);
            System.out.println("Deleted character...");
        }

        PlayerDAO playerDAO = new PlayerDAO();
        Player player = playerDAO.getById(123);
        if(player == null){
            System.out.println("No player found");
        }
        else {
            playerDAO.delete(player);
            System.out.println("Deleted player...");
        }

        DeveloperDAO developerDAO = new DeveloperDAO();
        Developer developer = developerDAO.getById(456);
        if(developer == null){
            System.out.println("No developer found");
        }
        else {
            developerDAO.delete(developer);
            System.out.println("Deleted developer...");
        }

        playerDAO = new PlayerDAO();
        player = playerDAO.getById(789);
        if(player == null){
            System.out.println("No user found");
        }
        else {
            playerDAO.delete(player);
            System.out.println("Deleted user...");
        }

        WeaponDAO weaponDAO = new WeaponDAO();
        Weapon weapon = new Weapon();

        weapon = weaponDAO.getById(1004);
        if(weapon != null){
            weaponDAO.delete(weapon);
            System.out.println("Deleted weapon");
        }
        else{
            System.out.println("No item found");
        }

    }
}
