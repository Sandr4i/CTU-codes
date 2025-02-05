package editor.bases;

import javax.swing.*;
import java.awt.*;

public class Frame extends JInternalFrame {
    public Container content;
    public Frame(String name, Dimension size, Point position){
        super(name, false, true, true, false);
        this.content = this.getContentPane();
        this.setSize(size);
        this.setLocation(position.x, position.y);
        this.setLayout(new BorderLayout(0,0));
        this.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
    }
    public void paintComponent(Graphics g){
        super.paintComponent(g);
    }
}
