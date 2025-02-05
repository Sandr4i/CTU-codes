package managers;

import main.GameSetup;

import java.awt.*;

public class EventManager {
    GameSetup setup;
    Rectangle eventBox;
    int defaultX, defaultY;
    public EventManager(GameSetup setup){
        this.setup = setup;
        eventBox = new Rectangle();
        defaultX = eventBox.x = 25;
        defaultY = eventBox.y = 25;
        eventBox.width = 10;
        eventBox.height = 10;

    }
    public boolean contact(String dir, int col, int row){
        boolean contact = false;
        setup.player.hitBox.x += setup.player.X;
        setup.player.hitBox.y += setup.player.Y;
        eventBox.x += col*setup.BLOCKSIZE;
        eventBox.y += row*setup.BLOCKSIZE;
        if ( setup.player.hitBox.intersects(eventBox)){
            if ( setup.player.dir.contentEquals(dir) || dir.contentEquals("any")){
                contact = true;
            }
        }
        setup.player.hitBox.x = setup.player.defaultHitX;
        setup.player.hitBox.y = setup.player.defaultHitY;
        eventBox.x = defaultX;
        eventBox.y = defaultY;
        return contact;
    }
    public void check(){
        if (contact("right",23, 4)){
            damageTest();
        }
        if (contact("up",23, 4)){
            damageTest();
        }
        if (contact("down",23, 4)){
            damageTest();
        }
        if (contact("left",23, 4)){
            damageTest();
        }
    }
    public void damageTest(){
        setup.player.health -= 1;
        System.out.println(("HIT!"));

    }
}
