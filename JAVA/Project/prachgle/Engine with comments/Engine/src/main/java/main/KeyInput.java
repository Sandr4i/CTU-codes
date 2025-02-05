package main;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import mainMenu.*;

/**
 * This class is for setting functions to the keys
 * @author doronmak, prachgle
 */
public class KeyInput implements KeyListener {
    public static boolean upPressed;
    public static boolean downPressed;
    public static boolean rightPressed;
    public static boolean leftPressed;
    public static boolean time;

    /**
     * For the future implementations
     * @param e the event to be processed
     */
    @Override
    public void keyTyped(KeyEvent e) {
    }

    /**
     * Determines pressed key
     * @param e the event to be processed
     */

    @Override
    public void keyPressed(KeyEvent e) {
        int key = e.getKeyCode();
        if (key == KeyEvent.VK_W){
            upPressed = true;
        }

        if (key == KeyEvent.VK_A){
            leftPressed = true;
        }

        if (key == KeyEvent.VK_S){
            downPressed = true;
        }

        if (key == KeyEvent.VK_D){
            rightPressed = true;
        }

        if (key == KeyEvent.VK_B){
            if (!time){
                time = true;
            }
            else if (time){
                time = false;
            }
        }

        if (key == KeyEvent.VK_ESCAPE){
            PlayDemoDefaultButton.close = true;
            PlayDemoCustomButton.close = true;
        }
    }

    /**
     * Conditions for released keys
     * @param e the event to be processed
     */

    @Override
    public void keyReleased(KeyEvent e) {
        int key = e.getKeyCode();
        if (key == KeyEvent.VK_W){
            upPressed = false;
        }

        if (key == KeyEvent.VK_A){
            leftPressed = false;
        }

        if (key == KeyEvent.VK_S){
            downPressed = false;
        }
        if (key == KeyEvent.VK_D){
            rightPressed = false;
        }
        if (key == KeyEvent.VK_ESCAPE){
            PlayDemoDefaultButton.close = false;
            PlayDemoCustomButton.close = false;
        }
    }
}
