package main;

import java.awt.*;
import javax.swing.JPanel;
import mainMenu.PlayDemoCustomButton;
import mainMenu.PlayDemoDefaultButton;
import managers.EventManager;
import managers.ObjectManager;
import managers.objects.Object;
import creatures.ContactCheck;
import creatures.Player;
import managers.TextureSetup;

/**
 * This clas is for
 * @author doronmak, prachgle
 */
public class GameSetup extends JPanel implements Runnable{

    static PlayDemoDefaultButton demo;
    static PlayDemoCustomButton demoCustom;
    public boolean custom;
    public String path;
    public int coordX;
    public int coordY;
    static final int textureSize = 16;
    static final int scale = 3;
    public String map;
    public static final int BLOCKSIZE = textureSize * scale;
    public static final int screenHeightBlocks = 16;
    public static final int screenWidthBlocks = 12;
    public static final int screenWidth = screenHeightBlocks * BLOCKSIZE;
    public static final int screenHeight = screenWidthBlocks * BLOCKSIZE;
    public int worldHeight;
    public int worldWidth;
    public ObjectManager om = new ObjectManager(this);
    public Object[] objects = new Object[15];
    public KeyInput input = new KeyInput();
    public Player player = new Player(this, input);
    public Interface ui = new Interface(this);
    public EventManager ev = new EventManager(this);
    public ContactCheck c = new ContactCheck(this);
    public Thread thd;
    public static TextureSetup txSetup;

    /**
     * Class constructor
     * @param custom        checks for map type
     * @param worldWidth    the width of the game map?
     * @param worldHeight   the height of the game map?
     * @param path          the path to the map location
     * @param coordX        the x coordinate of start position
     * @param coordY        the y coordinate of start position
     */

    public GameSetup(boolean custom, int worldWidth, int worldHeight, String path, int coordX, int coordY){
        this.custom = custom;
        this.setPreferredSize((new Dimension(screenWidth, screenHeight)));
        this.setBackground((Color.white));
        this.setDoubleBuffered(true);
        this.addKeyListener(input);
        this.setFocusable(true);
        setupMap(worldWidth, worldHeight,path,coordX, coordY );
    }

    /**
     *  Sets different params due to type of map
     * @param worldWidth    the width of the game map
     * @param worldHeight   the height of the game map
     * @param path          the path to the map location
     * @param coordX        the x coordinate of start position
     * @param coordY        the y coordinate of start position
     */

    public void setupMap(int worldWidth, int worldHeight, String path, int coordX, int coordY){
        if (this.custom){
            this.worldWidth = worldWidth;
            this.worldHeight = worldHeight;
            this.path = path;
            if (coordX < 0){
                this.coordX = worldWidth / 2;
                this.coordY = worldHeight / 2;
            }
            else{
                this.coordX = coordX;
                this.coordY = coordY;
            }
        }
        else {
            this.worldWidth = 50;
            this.worldHeight = 50;
            this.coordX = 23;
            this.coordY = 5;
        }
        if (this.custom){
            this.map = path;
            System.out.println(this.path);// add to above
        }
        else if (this.demo.mapIdx == 0 || this.demoCustom.mapIdx == 0 ) {
            this.worldHeight = 12;
            this.worldWidth = 16;
            this.map = "/maps/map0.txt";}
        else {
            this.worldHeight = 50;
            this.worldWidth = 50;
            this.map = "/maps/map1.txt";
        }

        System.out.println(this.map);
        if (this.path == null){
            txSetup = new TextureSetup(this, map);
        }
        else {
            txSetup = new TextureSetup(this, path);
        }
        this.player.setPlayer();
    }

    /**
     * Opens thread
     */

    public void GameThread(){
        thd = new Thread(this);
        thd.start();
    }

    /**
     * Makes update for player
     */

    public void update() { player.update();}

    /***
     * Runs time loop in which updates data and redraws characters
     */

    @Override
    public void run() {
        double interval = 1000000000/60; // 60 - frames per second
        double delta = 0;
        long lastTime = System.nanoTime();
        long currentTime;
        while (thd != null){
            currentTime = System.nanoTime();
            delta += (currentTime - lastTime)/interval;
            lastTime = currentTime;
            if (delta >= 1){
                update();
                repaint();
                delta--;
            }
        }
    }

    /**
     * Paints objects in game
     * @param g the <code>Graphics</code> object to protect
     */
    @Override
    public void paintComponent(Graphics g){
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D)g;
        long start = 0;
        if (input.time){
            start = System.nanoTime();
        }
        txSetup.draw(g2);
        for (int i = 0; i < objects.length; i++){
            if (objects[i] != null) {
                objects[i].draw(this, g2);}
        }
        player.draw(g2);
        ui.display(g2);
        if (input.time){
            long end = System.nanoTime();
            long res = end - start;
            g2.setColor(Color.WHITE);
            g2.drawString("Draw time: " + res, 10, 500);
        }
        g2.dispose();
    }

    /***
     * ???
     */

    public void setObjects() {om.loadObjects();}

}
