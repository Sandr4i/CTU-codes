package managers;

import main.GameSetup;
import managers.objects.*;

public class ObjectManager {
    GameSetup setup;
    public ObjectManager(GameSetup setup){this.setup = setup;}
    public void loadObjects(){
        setup.objects[0] = new doorKey(setup);
        setup.objects[0].X = 21 * setup.BLOCKSIZE;
        setup.objects[0].Y = 4 * setup.BLOCKSIZE;

        setup.objects[1] = new Door(setup);
        setup.objects[1].X = 19 * setup.BLOCKSIZE;
        setup.objects[1].Y = 4 * setup.BLOCKSIZE;

        setup.objects[2] = new Chest(setup);
        setup.objects[2].X = 20 * setup.BLOCKSIZE;
        setup.objects[2].Y = 4 * setup.BLOCKSIZE;

        setup.objects[3] = new frogKey(setup);
        setup.objects[3].X = 22 * setup.BLOCKSIZE;
        setup.objects[3].Y = 4 * setup.BLOCKSIZE;

        setup.objects[4] = new Hat(setup);
        setup.objects[4].X = 23 * setup.BLOCKSIZE;
        setup.objects[4].Y = 4 * setup.BLOCKSIZE;
    }
}
