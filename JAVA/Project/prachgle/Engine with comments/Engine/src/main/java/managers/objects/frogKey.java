package managers.objects;

import main.GameSetup;

import javax.imageio.ImageIO;
import java.io.IOException;

public class frogKey extends Object{
    GameSetup setup;
    public frogKey(GameSetup setup){
        this.setup = setup;
        name = "frogKey";
        try{
            image = ImageIO.read(getClass().getResourceAsStream("/objects/frog_key.png"));
            tb.processImage(image, setup.BLOCKSIZE, setup.BLOCKSIZE);
        } catch(IOException e){
            e.printStackTrace();
        }
    }
}
