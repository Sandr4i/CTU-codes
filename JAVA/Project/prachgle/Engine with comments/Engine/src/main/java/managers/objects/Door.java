package managers.objects;

import main.GameSetup;

import javax.imageio.ImageIO;
import java.io.IOException;

public class Door extends Object{
    GameSetup setup;
    public Door(GameSetup setup){
        this.setup = setup;
        name = "Door";
        try {
            image = ImageIO.read(getClass().getResourceAsStream("/objects/door.png"));
            tb.processImage(image, setup.BLOCKSIZE, setup.BLOCKSIZE);
        } catch(IOException e){
            e.printStackTrace();
        }
        contact = true;
    }
}
