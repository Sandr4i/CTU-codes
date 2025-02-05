package editor.main;

import editor.bases.Frame;
import editor.bases.Map;
import editor.editorWindow.MapEditor;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.IOException;

public class CreateMap extends Frame implements ActionListener {
    private int blockWidth = 16;
    private int blockHeight = 16;
    private JLabel mapWidth;
    private JLabel mapHeight;
    private JLabel imagePath;
    private JLabel name;
    private JTextField tMapWidth;
    private JTextField tMapHeight;
    private JTextField tImagePath;
    private JTextField tFileName;
    public String path;
    public int worldWidth;
    public int worldHeight;
    public MapEditor frame;
    private Container desktop;
    public CreateMap(Container desktop){
        super("Create a new map", new Dimension(500, 400), new Point(100, 100));
        this.desktop = desktop;
        createFrame();
    }

    public void createFrame(){
        JPanel panel = new JPanel(new GridLayout(7,2, 0, 5));
        JPanel buttonPanel = new JPanel(new FlowLayout(50, 10, 5));
        panel.setBorder(BorderFactory.createEmptyBorder(10,10,10,10));
        this.name = new JLabel("Name your map: ");
        this.mapWidth = new JLabel("Map width in blocks: ");
        this.mapHeight = new JLabel("Map height in blocks: ");
        this.tImagePath = new JTextField();
        this.tFileName = new JTextField();
        this.tMapWidth = new JTextField();
        this.tMapHeight = new JTextField();

        JButton save = new JButton("Save");
        JButton cancel = new JButton("Cancel");


        panel.add(this.name);
        panel.add(this.tFileName);
        panel.add(this.mapWidth);
        panel.add(this.tMapWidth);
        panel.add(this.mapHeight);
        panel.add(this.tMapHeight);
        buttonPanel.add(save);
        buttonPanel.add(cancel);

        save.addActionListener(this);
        cancel.addActionListener(this);
        this.content.add(panel, BorderLayout.NORTH);
        this.content.add(buttonPanel, BorderLayout.SOUTH);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String command = e.getActionCommand();
        ClassLoader classLoader = getClass().getClassLoader();
        String folderPath = classLoader.getResource("maps").getPath();
        folderPath += "/";
        String relativePath = folderPath.replace("target/classes/", "src/main/resources/");
        if(command.equalsIgnoreCase("Save")){
            int width = Integer.parseInt(this.tMapWidth.getText()) * blockWidth;
            int height = blockHeight * Integer.parseInt(this.tMapHeight.getText());

            path = relativePath + tFileName.getText();
            Map map = new Map(path+".png",
                    blockWidth,
                    blockHeight,
                    Integer.parseInt(this.tMapWidth.getText()),
                    Integer.parseInt(this.tMapHeight.getText()));
            Component[] frames = desktop.getComponents();
            for(Component c : frames){
                if (c instanceof MapEditor){
                    MapEditor mFrame = (MapEditor)c;
                    mFrame.dispose();
                }
            }
            frame = new MapEditor(map, desktop.getSize(), width, height);
            desktop.add(frame);
            frame.setVisible(true);
            worldWidth = frame.editorPanel.width/3;
            worldHeight = frame.editorPanel.height/3;
            this.dispose();
        }
        else if(command.equalsIgnoreCase("Cancel")){
            this.dispose();
        }
    }
}
