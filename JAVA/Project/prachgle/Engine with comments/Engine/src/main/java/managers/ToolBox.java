package managers;

import java.awt.*;
import java.awt.image.BufferedImage;

public class ToolBox {
    public BufferedImage processImage(BufferedImage org, int width, int height){
        BufferedImage sc = new BufferedImage(width, height, org.getType());
        Graphics2D g = sc.createGraphics();
        g.drawImage(org, 0, 0, width, height, null);
        g.dispose();
        return sc;
    }
}
