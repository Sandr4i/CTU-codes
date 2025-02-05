package managers.objects;

import main.GameSetup;
import managers.ToolBox;

import javax.imageio.ImageIO;
import java.io.IOException;

public class Hat extends Object{
    GameSetup setup;
    public Hat(GameSetup setup) {
        this.setup = setup;
        name = "Hat";
        try{
            image = ImageIO.read(getClass().getResourceAsStream("/objects/hat.png"));
            tb.processImage(image, setup.BLOCKSIZE, setup.BLOCKSIZE);
        }
        catch (IOException e){
            e.printStackTrace();
        }
    }
}
