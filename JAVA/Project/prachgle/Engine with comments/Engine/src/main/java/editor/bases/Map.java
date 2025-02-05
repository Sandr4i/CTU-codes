package editor.bases;

import java.io.Serializable;

public class Map implements Serializable {

    public String path;
    public int mapWidth;
    public int mapHeight;
    public int blockWidth;
    public int blockHeight;
    public int[][] textures;
    public Map(){
        this.path = "";
        this.mapWidth = 0;
        this.mapHeight = 0;
        this.blockWidth = 0;
        this.blockHeight = 0;
        this.textures = new int[0][0];
    }

    public Map(String path, int tWidth, int tHeight, int mapWidth, int mapHeight){
        this.path = path;
        this.blockWidth = tWidth;
        this.blockHeight = tHeight;
        this.mapWidth = mapWidth;
        this.mapHeight = mapHeight;
        this.textures = new int[mapHeight][mapWidth];

        for (int i = 0; i < mapHeight; i++){
            for (int j = 0; j < mapWidth; j++){
                this.textures[i][j] = 0;
            }
        }
    }

}
