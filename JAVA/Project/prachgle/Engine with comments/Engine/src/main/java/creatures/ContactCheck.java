package creatures;

import main.GameSetup;

import java.util.logging.Level;
import java.util.logging.Logger;

import static main.GameSetup.BLOCKSIZE;
import static managers.TextureSetup.mapTextures;
import static managers.TextureSetup.texture;

/**
 * This class is for setting collision
 * and determination of possibility to take objects
 * @author prachgle
 */

public class ContactCheck {
    GameSetup setup;
    private static final Logger logger = Logger.getLogger(ContactCheck.class.getName());
    public ContactCheck(GameSetup setup) {this.setup = setup;}
    public void checkBlock(Creature creature){
        int leftWorldX = creature.X + creature.hitBox.x;
        int rightWorldX = creature.X + creature.hitBox.x + creature.hitBox.width;
        int topWorldY = creature.Y + creature.hitBox.y;
        int bottomWorldY = creature.Y + creature.hitBox.y + creature.hitBox.height;
        int leftCol = leftWorldX/ BLOCKSIZE;
        int rightCol = rightWorldX/ BLOCKSIZE;
        int topRow = topWorldY/BLOCKSIZE;
        int bottomRow = bottomWorldY/ BLOCKSIZE;
        int i;
        int j;
        switch (creature.dir) {
            case ("up") -> {
                topRow = (topWorldY - creature.moveSpeed) / BLOCKSIZE;
                i = mapTextures[leftCol][topRow];
                j = mapTextures[rightCol][topRow];
                if (texture[i].contact || texture[j].contact) {creature.contact = true;}
            }
            case ("down") -> {
                bottomRow = (bottomWorldY + creature.moveSpeed) / BLOCKSIZE;
                i = mapTextures[leftCol][bottomRow];
                j = mapTextures[rightCol][bottomRow];
                if (texture[i].contact || texture[j].contact) {creature.contact = true;}
            }
            case ("right") -> {
                rightCol = (rightWorldX + creature.moveSpeed) / BLOCKSIZE;
                i = mapTextures[rightCol][topRow];
                j = mapTextures[rightCol][bottomRow];
                if (texture[i].contact || texture[j].contact) {creature.contact = true;}
            }
            case ("left") -> {
                leftCol = (leftWorldX - creature.moveSpeed) / BLOCKSIZE;
                i = mapTextures[leftCol][topRow];
                j = mapTextures[leftCol][bottomRow];
                if (texture[i].contact || texture[j].contact) {creature.contact = true;}
            }
            default ->
                    logger.log(Level.WARNING, "Error! Unknown direction! ");
        }
    }

    /**
     *
     * @param creature represents player???
     * @param player condition for taking by player
     * @return possibility to take the object
     */
    public int checkObject(Creature creature, boolean player){
        int i = -1;
        int j = 0;
        while (j < setup.objects.length) {
            if (setup.objects[j] != null){
                creature.hitBox.x += creature.X;
                creature.hitBox.y += creature.Y;
                setup.objects[j].hitBox.x += setup.objects[j].X;
                setup.objects[j].hitBox.y += setup.objects[j].Y;
                switch (creature.dir) {
                    case ("up") -> {
                        creature.hitBox.y -= creature.moveSpeed;
                        if (creature.hitBox.intersects(setup.objects[j].hitBox)) {
                            if (setup.objects[j].contact) {creature.contact = true;}
                            if (player) {i = j;}
                        }
                    }
                    case ("down") -> {
                        creature.hitBox.y += creature.moveSpeed;
                        if (creature.hitBox.intersects(setup.objects[j].hitBox)) {
                            if (setup.objects[j].contact) {creature.contact = true;}
                            if (player) {i = j;}
                        }
                    }
                    case ("right") -> {
                        creature.hitBox.x += creature.moveSpeed;
                        if (creature.hitBox.intersects(setup.objects[j].hitBox)) {
                            if (setup.objects[j].contact) {creature.contact = true;}
                            if (player) {i = j;}
                        }
                    }
                    case ("left") -> {
                        creature.hitBox.x -= creature.moveSpeed;
                        if (creature.hitBox.intersects(setup.objects[j].hitBox)) {
                            if (setup.objects[j].contact) {creature.contact = true;}
                            if (player) {i = j;}
                        }
                    }
                    default ->
                        logger.log(Level.WARNING, "Error! Unknown direction! ");

                }
                creature.hitBox.x = creature.defaultHitX;
                creature.hitBox.y = creature.defaultHitY;
                setup.objects[j].hitBox.x = setup.objects[j].hitX;
                setup.objects[j].hitBox.y = setup.objects[j].hitY;
            }
            j++;
        }
        return i;
    }
}
