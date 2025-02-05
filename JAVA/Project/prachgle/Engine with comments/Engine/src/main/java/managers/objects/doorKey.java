package managers.objects;

import main.GameSetup;

import javax.imageio.ImageIO;
import java.io.IOException;

public class doorKey extends Object{
    GameSetup setup;
    public doorKey(GameSetup setup){
        this.setup = setup;
        name = "doorKey";
        try{
            image = ImageIO.read(getClass().getResourceAsStream("/objects/key.png"));
            tb.processImage(image, setup.BLOCKSIZE, setup.BLOCKSIZE);
        } catch(IOException e){
            e.printStackTrace();
        }
    }
}
