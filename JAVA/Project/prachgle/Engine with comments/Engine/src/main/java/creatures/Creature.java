package creatures;

import java.awt.*;
import java.awt.image.BufferedImage;

/**
 * This class is for setting creature's params
 * @author prachgle
 */

public class Creature {
    public static int X;
    public static int Y;
    public static int moveSpeed;
    public static String dir;
    public static Rectangle hitBox;
    public static int maxHealth;
    public static int health;
    public static int defaultHitX;
    public static int defaultHitY;
    public static BufferedImage up1;
    public static BufferedImage up2;
    public static BufferedImage up3;
    public static BufferedImage down1;
    public static BufferedImage down2;
    public static BufferedImage down3;
    public static BufferedImage left1;
    public static BufferedImage left2;
    public static BufferedImage left3;
    public static BufferedImage right1;
    public static BufferedImage right2;
    public static BufferedImage right3;
    public static int modelCount = 0;
    public static int modelNum = 1;
    public static boolean contact = false;
}
