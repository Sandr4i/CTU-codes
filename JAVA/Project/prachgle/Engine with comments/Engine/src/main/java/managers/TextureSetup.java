package managers;

import main.GameSetup;

import javax.imageio.ImageIO;
import java.awt.*;
import java.io.*;

public class TextureSetup implements Serializable {
    GameSetup setup;
    public static Texture[] texture;
    public static int[][] mapTextures;

    public TextureSetup(GameSetup setup, String map){
        this.setup = setup;
        texture = new Texture[50];
        mapTextures = new int[setup.worldWidth][setup.worldHeight];
        getTextures();
        loadMap(map);
    }

    private void getTextures(){
        // I wanted to try load textures with loop, but it would have made a mess
        // and it would be difficult to use certain texture, so i load textures manually
        set("grass_1.png", 10, false);
        set("grass_2.png", 6, false);
        set("grass_3.png", 13, false);
        set("water_1.png", 5, false);
        set("water_2.png", 3, false);
        set("water_3.png", 1, false);
        set("tall_grass_1.png", 4, true);
        set("tall_grass_2.png", 7, true);
        set("tall_grass_3.png", 2, true);
        set("bush.png", 11, true);
        set("sand.png", 9, false);
        set("dirt.png", 14, false);
        set("sea.png", 8, true);
        set("sea_2.png", 12, true);
    }
    public void set(String name, int id, boolean contact){
        ToolBox tb = new ToolBox();
        try{
            texture[id] = new Texture();
            texture[id].image = ImageIO.read(getClass().getResourceAsStream("/textures/"+name));
            texture[id].image = tb.processImage(texture[id].image, setup.BLOCKSIZE, setup.BLOCKSIZE);
            texture[id].contact = contact;
        }
        catch (IOException e){
            e.printStackTrace();
        }
    }
    private void loadMap(String map){
        System.out.println("Map: " + map);
        try {
            InputStream m = getClass().getResourceAsStream(map);
            BufferedReader r = new BufferedReader(new InputStreamReader(m));
            int h = 0;
            int w = 0;
            while (h < setup.worldWidth && w < setup.worldHeight){
                String line = r.readLine();
                while(h < setup.worldWidth){
                    String idx[] = line.split(" ");
                    int num = Integer.parseInt(idx[h]);
                    mapTextures[h][w] = num;
                    h++;
                }
                if ( h == setup.worldWidth){
                    h = 0;
                    w++;
                }
            }
            r.close();
        } catch (Exception e){}
    }
    public void draw(Graphics2D g){
        int h = 0;
        int w = 0;
        //System.out.println("World width: " + setup.worldWidth + " World Height: " + setup.worldHeight);
        while (h < setup.worldWidth && w < setup.worldHeight){
            int txIdx = mapTextures[h][w];
            int worldX = h * setup.BLOCKSIZE;
            int worldY = w * setup.BLOCKSIZE;
            int screenX = worldX - setup.player.X + setup.player.screenX;
            int screenY = worldY - setup.player.Y + setup.player.screenY;
            if (worldX + setup.BLOCKSIZE > setup.player.X - setup.player.screenX &&
                worldX - setup.BLOCKSIZE < setup.player.X + setup.player.screenX &&
                worldY + setup.BLOCKSIZE > setup.player.Y - setup.player.screenY &&
                worldY - setup.BLOCKSIZE < setup.player.Y + setup.player.screenY){
                g.drawImage(texture[txIdx].image, screenX, screenY, null);
            }
            h++;
            if (h == setup.worldWidth){
                h = 0;
                w++;
            }
        }
    }
}
