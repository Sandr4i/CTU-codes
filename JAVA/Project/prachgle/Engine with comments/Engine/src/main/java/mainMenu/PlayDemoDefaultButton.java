package mainMenu;

import main.GameSetup;

import javax.swing.*;
import java.awt.*;
import java.io.Serializable;

public class PlayDemoDefaultButton implements Serializable {
    public static int mapIdx = 1;
    public static boolean close = false;
    public static boolean custom = false;
    public void show(){
        JFrame w = new JFrame();
        w.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        w.setResizable(false);
        w.setTitle("Demo");
        GameSetup setup = new GameSetup(false, 0, 0, "", 0, 0);
        //setup.gm.setupMap();
        System.out.println(setup.worldWidth + " " + setup.worldHeight+ " " + setup.map+ " " + setup.coordX + " " + setup.coordY);
        w.add(setup);
        w.pack();
        w.setLocationRelativeTo(null);
        w.setVisible(true);
        setup.setObjects();
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
