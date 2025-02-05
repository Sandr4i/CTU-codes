package managers.objects;

import main.GameSetup;

import javax.imageio.ImageIO;
import java.io.IOException;

public class Health extends Object{
    GameSetup setup;
    public Health(GameSetup setup) {
        this.setup = setup;
        name = "Health";
        try{
            image = ImageIO.read(getClass().getResourceAsStream("/ui/life_1.png"));
            image2 = ImageIO.read(getClass().getResourceAsStream("/ui/life_2.png"));
            image3 = ImageIO.read(getClass().getResourceAsStream("/ui/life_3.png"));
            image = tb.processImage(image, setup.BLOCKSIZE, setup.BLOCKSIZE);
            image2 = tb.processImage(image2, setup.BLOCKSIZE, setup.BLOCKSIZE);
            image3 = tb.processImage(image3, setup.BLOCKSIZE, setup.BLOCKSIZE);
        }
        catch (IOException e){
            e.printStackTrace();
        }
    }
}
