package managers.objects;

import main.GameSetup;
import managers.ToolBox;

import java.awt.*;
import java.awt.image.BufferedImage;

public class Object {
    ToolBox tb = new ToolBox();
    public int X, Y;
    public int hitX = 0, hitY = 0;
    public String name;
    public BufferedImage image, image2, image3;
    public Rectangle hitBox = new Rectangle(0,0, 48, 48);
    public boolean contact = false;
    public void draw(GameSetup setup, Graphics2D g){
        int scX = X - setup.player.X + setup.player.screenX;
        int scY = Y - setup.player.Y + setup.player.screenY;
        if (X + setup.BLOCKSIZE > setup.player.X - setup.player.screenX &&
            X - setup.BLOCKSIZE < setup.player.X + setup.player.screenX &&
            Y + setup.BLOCKSIZE > setup.player.Y - setup.player.screenY &&
            Y - setup.BLOCKSIZE < setup.player.Y + setup.player.screenY){
            g.drawImage(image, scX, scY, setup.BLOCKSIZE, setup.BLOCKSIZE, null);
        }
    }
}
