package creatures;

import main.GameSetup;
import main.KeyInput;
import managers.ToolBox;
import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import static main.GameSetup.*;
import static main.KeyInput.*;

/**
 * This class is for making changing in player movements,
 * textures, stats, entities
 * @author doronmak, prachgle
 */

public class Player extends Creature{
    private static final Logger logger = Logger.getLogger(Player.class.getName());
    GameSetup setup;
    KeyInput input;
    public static int screenX;
    public static int screenY;
    public static int frogKeys = 0;
    public static int doorKeys = 0;

    /**
     * @param setup contains data from GameSetup
     * @param input contains data from KeyInput
     */
    public Player(GameSetup setup, KeyInput input){
        this.setup = setup;
        this.input = input;
        screenX = (screenWidth/2) - (BLOCKSIZE /2);
        screenY = (screenHeight/2) - (BLOCKSIZE /2);
        hitBox = new Rectangle();
        defaultHitX = 6;
        defaultHitY = 12;
        hitBox.x= defaultHitX;
        hitBox.y = defaultHitY;
        hitBox.width = 16;
        hitBox.height = 16;
        getPlayerModels();
    }

    /**
     * Contains player's stats
     */
    public void setPlayer(){
        maxHealth = 6;
        health = maxHealth;
        dir = "down";
        moveSpeed = 4;
        X = BLOCKSIZE * this.setup.coordX;
        Y = BLOCKSIZE * this.setup.coordY;
    }

    /**
     * Contains player's textures
     */

    public void getPlayerModels(){
        up1 = set("frog_back_1.png");
        up2 = set("frog_back_2.png");
        up3 = set("frog_back_3.png");
        down1 = set("frog_front_1.png");
        down2 = set("frog_front_2.png");
        down3 = set("frog_front_3.png");
        right1 = set("frog_right_1.png");
        right2 = set("frog_right_2.png");
        right3 = set("frog_right_3.png");
        left1 = set("frog_left_1.png");
        left2 = set("frog_left_2.png");
        left3 = set("frog_left_3.png");
    }

    /**
     * Sets images
     * @param name  The name of the image
     * @return Result of the processing image
     */
    public BufferedImage set(String name){
        ToolBox tb = new ToolBox();
        BufferedImage res = null;
        try{
            res = ImageIO.read(getClass().getResourceAsStream("/playerModel/" + name));
            res = tb.processImage(res, BLOCKSIZE, BLOCKSIZE);
        }
        catch (IOException e){
            e.printStackTrace();
        }
        return res;
    }

    /**
     * Draws player's movements
     */
    public void update(){
        if (upPressed || downPressed || leftPressed || rightPressed){
            if (upPressed) {dir = "up";}
            else if (downPressed) {dir = "down";}
            else if (leftPressed) { dir = "left";}
            else { dir = "right";}
            contact = false;
            setup.c.checkBlock(this);
            int i = setup.c.checkObject(this, true);
            pickUp(i);
            setup.ev.check();
            if (!contact) {
                switch (dir) {
                    case ("up") -> Y -= moveSpeed;
                    case ("down") -> Y += moveSpeed;
                    case ("left") -> X -= moveSpeed;
                    case ("right") -> X += moveSpeed;
                    default -> logger.log(Level.WARNING, "Error! Unknown direction! ");
                }
            }
            modelCount++;
            if (modelCount>12){
                if (modelNum == 1){modelNum = 2;}
                else if (modelNum == 2) {modelNum = 3;}
                else if (modelNum == 3) {modelNum = 1;}
                modelCount = 0;
            }
        }
    }

    /**
     *  Chooses images
     * @param g Contains graphic components
     */
    public void draw(Graphics2D g){
        BufferedImage image = null;
        switch (dir) {
            case "up" -> {
                if (modelNum == 1) {
                    image = up1;
                } else if (modelNum == 2) {
                    image = up2;
                } else if (modelNum == 3) {
                    image = up3;
                }
            }
            case "down" -> {
                if (modelNum == 1) {
                    image = down1;
                } else if (modelNum == 2) {
                    image = down2;
                } else if (modelNum == 3) {
                    image = down3;
                }
            }
            case "right" -> {
                if (modelNum == 1) {
                    image = right1;
                } else if (modelNum == 2) {
                    image = right2;
                } else if (modelNum == 3) {
                    image = right3;
                }
            }
            case "left" -> {
                if (modelNum == 1) {
                    image = left1;
                } else if (modelNum == 2) {
                    image = left2;
                } else if (modelNum == 3) {
                    image = left3;
                }
            }
            default -> logger.log(Level.WARNING, "Error! Unknown direction! ");

        }
        g.drawImage(image, screenX, screenY, null);
    }

    /**
     * Sets number of entities
     * @param id ???
     */
    public void pickUp(int id){
        if (id != -1){
            switch (setup.objects[id].name) {
                case ("doorKey") -> {
                    doorKeys++;
                    setup.objects[id] = null;
                }
                case ("Door") -> {
                    if (doorKeys > 0) {
                        setup.objects[id] = null;
                        doorKeys--;
                    }
                }
                case ("frogKey") -> {
                    frogKeys++;
                    setup.objects[id] = null;
                }
                case ("Chest") -> {
                    if (frogKeys > 0) {
                        setup.objects[id] = null;
                        frogKeys--;
                    }
                }
                case ("Hat") -> {
                    maxHealth += 2;
                    health = maxHealth;
                    setup.objects[id] = null;
                }
                default -> logger.log(Level.WARNING, "Error! Unknown object! ");
            }
        }
    }
}
