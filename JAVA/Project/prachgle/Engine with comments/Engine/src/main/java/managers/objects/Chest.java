package managers.objects;

import main.GameSetup;

import javax.imageio.ImageIO;
import java.io.IOException;

public class Chest extends Object{
    GameSetup setup;
    public Chest(GameSetup setup){
        this.setup = setup;
        name = "Chest";
        try {
            image = ImageIO.read(getClass().getResourceAsStream("/objects/chest.png"));
            tb.processImage(image, setup.BLOCKSIZE, setup.BLOCKSIZE);
        } catch(IOException e){
            e.printStackTrace();
        }
        contact = true;
    }
}
