package main;

import managers.objects.Health;
import managers.objects.Object;
import managers.objects.doorKey;
import managers.objects.frogKey;
import java.awt.*;
import java.awt.image.BufferedImage;
import static main.GameSetup.BLOCKSIZE;

/**
 * This class is for setting images to the game objects
 * @author doronmak, prachgle
 */

public class Interface {
    GameSetup setup;
    Graphics2D g;
    private Font font = new Font("Arial", Font.ITALIC, 36);
    private BufferedImage doorKeyImg;
    private BufferedImage frogKeyImg;
    private BufferedImage healthFull;
    private BufferedImage healthHalf;
    private BufferedImage healthEmpty;

    /**
     * Class constructor
     * @param setup contains game objects
     */

     public Interface(GameSetup setup){
        this.setup = setup;
         doorKey dkey = new doorKey(setup);
         doorKeyImg = dkey.image;
         frogKey fkey = new frogKey(setup);
         frogKeyImg = fkey.image;
         Object health = new Health(setup);
         healthFull = health.image;
         healthHalf = health.image2;
         healthEmpty = health.image3;
    }

    /**
     * Sets game screen
     * @param g game graphics
     */

    public void display(Graphics2D g){
         this.g = g;
        g.setFont(font);
        g.setColor(Color.WHITE);
        g.drawImage(doorKeyImg, BLOCKSIZE/2, BLOCKSIZE/2, BLOCKSIZE, BLOCKSIZE, null);
        g.drawString("x " + setup.player.doorKeys, 74, 60);
        g.setColor(Color.WHITE);
        g.setFont(font);
        g.drawImage(frogKeyImg, BLOCKSIZE/2, BLOCKSIZE*2, BLOCKSIZE, BLOCKSIZE, null);
        g.drawString("x " + setup.player.frogKeys, 74, 120);
        drawHealth();
    }

    /**
     * Draws health indication
     */

    public void drawHealth(){
         int x = BLOCKSIZE/2-20;
         int y = BLOCKSIZE*11-5;
         int i = 0;

         while ( i < setup.player.maxHealth/2){
             g.drawImage(healthEmpty, x, y, null);
             i++;
             x += BLOCKSIZE+5;
         }

        x = BLOCKSIZE/2-20;
        y = BLOCKSIZE*11-5;
        i = 0;

        while (i < setup.player.health){
            g.drawImage(healthHalf
                    , x, y, null);
            i++;

            if (i < setup.player.health){
                g.drawImage(healthFull, x, y, null);
            }

            i++;
            x += BLOCKSIZE+5;
        }
    }
}
