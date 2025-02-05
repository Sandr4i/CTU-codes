package mainMenu;

import main.GameSetup;

import javax.swing.*;
import java.awt.*;

public class PlayDemoCustomButton {
    public static int mapIdx = 1;
    public static boolean close = false;
    public static boolean custom = true;
    public int worldWidth;
    public int worldHeight;
    public int coordX, coordY;
    public String path;
    public void show(){
        JFrame w = new JFrame();
        w.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        w.setResizable(false);
        w.setTitle("Demo");
        this.path = this.path.replace("/home/sandrai/Desktop/JAVA/Project/prachgle/Engine/src/main/resources", "") + ".txt";
        this.worldHeight = this.worldHeight/16;
        this.worldWidth = this.worldWidth / 16;
        GameSetup setup = new GameSetup(true, this.worldWidth, this.worldHeight, this.path, this.coordX - 8, this.coordY - 1);
        System.out.println(this.worldWidth + " " + this.worldHeight+ " " + this.path+ " " + setup.coordX + " " + setup.coordY);
        System.out.println("For setup: " + setup.custom + " " + setup.worldWidth + " " + setup.worldHeight+ " " + setup.path + " " + setup.coordX + " " + setup.coordY);
        w.add(setup);
        w.pack();
        w.setLocationRelativeTo(null);
        w.setVisible(true);
        setup.GameThread();
        while (!close && w.isVisible()) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        if (close) {
            System.out.println("ESC pressed");
            setup.thd.interrupt();
            w.dispose();
            close = false;
        }
    }
}
