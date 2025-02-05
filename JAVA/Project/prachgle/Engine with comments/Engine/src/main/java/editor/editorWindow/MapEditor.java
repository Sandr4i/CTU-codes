package editor.editorWindow;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.sql.SQLOutput;
import java.util.ArrayList;
import java.util.List;
import javax.imageio.ImageIO;
import javax.swing.*;

import editor.bases.Frame;
import editor.bases.Map;
import editor.bases.Picture;

public class MapEditor extends Frame {
    public Map current;
    public int choice;
    public BufferedImage mapImage;
    public Picture[] textures;
    public JScrollPane editorScrollBar;
    public JScrollPane texturesScrollBar;
    public EditorPanel editorPanel;
    public TexturePanel texturePanel;
    public Tools tools;

    public MapEditor(Map current, Dimension size, int width, int height) {
        super("Map editor", size, new Point(0, 0));
        this.current = current;
        this.choice = 0;


        Dimension imagePanelDimension = new Dimension(width * 25, height * 25);
        System.out.println("NewImage size : " + imagePanelDimension.getWidth() + ", " + imagePanelDimension.getHeight());

        this.editorPanel = new EditorPanel(width, height);
        this.editorPanel.setBackground(Color.BLACK);
        this.editorPanel.setPreferredSize(new Dimension(imagePanelDimension.width,
                imagePanelDimension.height - (imagePanelDimension.height/3)));

        this.texturePanel = new TexturePanel(this.editorPanel);
        this.texturePanel.setBackground(Color.WHITE);
        this.texturePanel.setPreferredSize(new Dimension((current.blockWidth) * 15,
                current.blockHeight * 30));

        this.tools = new Tools(this.editorPanel);
        this.tools.setBackground(Color.BLACK);
        this.tools.setPreferredSize(new Dimension(100, 150));

        this.editorScrollBar = new JScrollPane(this.editorPanel,
                JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        this.editorScrollBar.getViewport().setScrollMode(JViewport.BACKINGSTORE_SCROLL_MODE);
        this.editorScrollBar.setWheelScrollingEnabled(true);
        this.editorScrollBar.getVerticalScrollBar().setUnitIncrement(16);
        this.add(this.editorScrollBar, BorderLayout.CENTER);

        this.texturesScrollBar = new JScrollPane(this.texturePanel,
                JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        this.texturesScrollBar.getViewport().setScrollMode(JViewport.BACKINGSTORE_SCROLL_MODE);
        this.texturesScrollBar.setWheelScrollingEnabled(true);
        this.texturesScrollBar.getVerticalScrollBar().setUnitIncrement(16);
        this.add(this.texturesScrollBar, BorderLayout.EAST);

        JSplitPane split1 = new JSplitPane(JSplitPane.VERTICAL_SPLIT, this.tools, this.texturesScrollBar);
        split1.setOneTouchExpandable(true);
        split1.setResizeWeight(1.0);
        split1.setDividerSize(0);
        split1.setDividerLocation(150);
        this.add(split1);

        JSplitPane split2 = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, this.editorScrollBar, split1);
        split2.setOneTouchExpandable(true);
        split2.setResizeWeight(1.0);
        split2.setDividerSize(0);
        split2.setDividerLocation(1400);
        this.add(split2);

    }

    public class EditorPanel extends JPanel implements MouseListener, MouseMotionListener {
        public int width;
        public int height;
        public String action = "Draw";
        public ImageIcon tile;
        public ImageIcon[][] tiles;
        public ImageIcon[][] objectsTextures;
        public int number;
        private int gridSize;
        public int[][] grid;
        public int[][] objects;
        private int gridWidth;
        private int gridHeight;
        private Point selectedCell;
        private Point cell;
        private Point objectCell;
        private boolean playerDrawn = false;
        private boolean pressed = false;
        public int playerID = 0;
        public ImageIcon playerImage;
        public boolean player = false;
        public boolean object = false;
        public int playerX = -1;
        public int playerY = -1;

        public EditorPanel(int width, int height){
            super(new BorderLayout());
            this.setOpaque(true);
            this.height = height * 3;
            this.width = width * 3;

            addMouseListener(this);
            addMouseMotionListener(this);
            getMapSize();
        }
        public void getMapSize(){
            gridWidth = width;
            gridHeight = height;
            System.out.println("Width: " + this.width + " Height: " + this.height);
            gridSize = current.blockWidth * 3;
            grid = new int[(int)Math.ceil((double)gridWidth / gridSize)][(int)Math.ceil((double)gridHeight / gridSize)];
            objects = new int[(int)Math.ceil((double)gridWidth / gridSize)][(int)Math.ceil((double)gridHeight / gridSize)];
            tiles = new ImageIcon[(int)Math.ceil((double)gridWidth / gridSize)][(int)Math.ceil((double)gridHeight / gridSize)];
            objectsTextures = new ImageIcon[(int)Math.ceil((double)gridWidth / gridSize)][(int)Math.ceil((double)gridHeight / gridSize)];

        }

        public void paintComponent(Graphics g){
            super.paintComponent(g);
            Graphics2D g2 = (Graphics2D) g;
            drawTextures(g2);
            drawPlayer(g2);
            drawObjects(g2);
            selectCell(g2);
        }

        public void drawTextures(Graphics2D g2){
            Color gridColor = Color.WHITE;
            g2.setColor(gridColor);
            for(int i = 8; i < (int)Math.ceil((double)gridWidth / gridSize) + 8; i++) {
                for (int j = 1; j < (int) Math.ceil((double) gridHeight / gridSize) + 1; j++) {
                    int x = i * gridSize;
                    int y = j * gridSize;
                    g2.drawRect(x, y, gridSize, gridSize);
                    if (tiles[i-8][j-1] != null) {
                        Image img = tiles[i-8][j-1].getImage();
                        g2.drawImage(img, x, y, gridSize, gridSize, null);
                    }
                    if (pressed){
                        if (selectedCell != null) {
                            int imageX = selectedCell.x * gridSize;
                            int imageY = selectedCell.y * gridSize;
                            Image img = tile.getImage();
                            g2.drawImage(img, imageX, imageY, gridSize, gridSize, this);
                        }
                    }
                }
            }
        }

        public void drawPlayer(Graphics2D g){
            if (playerDrawn && playerX != -1 && playerY != -1){
                int x = playerX * gridSize;
                int y = playerY * gridSize;
                Image img = playerImage.getImage();
                g.drawImage(img, x, y, gridSize, gridSize, this);
            }
        }

        public void drawObjects(Graphics2D g){
//            for(int i = 8; i < (int)Math.ceil((double)gridWidth / gridSize) + 8; i++) {
//                for (int j = 1; j < (int) Math.ceil((double) gridHeight / gridSize) + 1; j++) {
//                    int x = i * gridSize;
//                    int y = j * gridSize;
//
//                    if (objectsTextures[i-8][j-1] != null){
//                        Image img = objectsTextures[i-8][j-1].getImage();
//                        g.drawImage(img, x, y, gridSize, gridSize, null);
//                    }
//                    if (objectCell != null){
//                        int imageX = objectCell.x * gridSize;
//                        int imageY = objectCell.y * gridSize;
//                        Image img = tile.getImage();
//                        g.drawImage(img, imageX, imageY, gridSize, gridSize, this);
//                    }
//                }
//            }
        }
        public void selectCell(Graphics2D g){
            if (cell != null){
                int x = cell.x * gridSize;
                int y = cell.y * gridSize;
                Color translucentColor = new Color(170, 150, 250, 150);
                g.setColor(translucentColor);
                g.fillRect(x, y, gridSize, gridSize);
            }
        }


        @Override
        public void mouseClicked(MouseEvent e) {
            int x = e.getX() / gridSize;
            int y = e.getY() / gridSize;

            if (action.equalsIgnoreCase("Draw")){
                if (x >= 8 && x < (int)Math.ceil((double)gridWidth / gridSize) + 8 && y >= 1 && y < (int)Math.ceil((double)gridHeight / gridSize) + 1){
                    if (player){
                        playerDrawn = true;
                        playerX = x;
                        playerY = y;
                    }
//                    else if (object){
//                        objectCell = new Point(x, y);
//                        objectsTextures[x-8][y-1] = tile;
//                        objects[x-8][y-1] = number;
//                    }
                    else{
                        selectedCell = new Point(x, y);
                        tiles[x-8][y-1] = tile;
                        grid[x-8][y-1] = number;
                    }
                }
            }
            else if (action.equalsIgnoreCase("Delete")){
                if (x >= 8 && x < (int)Math.ceil((double)gridWidth / gridSize) + 8 && y >= 1 && y < (int)Math.ceil((double)gridHeight / gridSize) + 1){
                    if (x == playerX && y == playerY){
                        playerDrawn = false;
                        playerX = -1;
                        playerY = -1;
                    }
//                    else if (object){
//                        objectCell = null;
//                        objectsTextures[x-8][y-1] = null;
//                        objects[x-8][y-1] = 0;
//                    }
                    else{
                        selectedCell = null;
                        tiles[x - 8][y - 1] = null;
                        grid[x - 8][y - 1] = 0;
                    }
                }
            }
            repaint();
            for (int i = 0; i < grid.length; i++){
                for (int j = 0; j < grid[0].length; j++){
                    System.out.print(grid[i][j] + " ");
                }
                System.out.print("\n");
            }
            System.out.println("Player selected: " + player + " Player ID: " + playerID + " Player x and y: " + "[ " + playerX + " ; " + playerY + " ]");
            System.out.print("\n\n");
        }

        @Override
        public void mouseExited(MouseEvent e) {
            cell = null;
            repaint();
        }

        @Override
        public void mouseMoved(MouseEvent e) {
            int x = e.getX() / gridSize;
            int y = e.getY() / gridSize;

            if (x >= 8 && x < (int)Math.ceil((double)gridWidth / gridSize) + 8 && y >= 1 && y < (int)Math.ceil((double)gridHeight / gridSize) + 1) {
                cell = new Point(x, y);
                repaint();
            }
        }
        @Override
        public void mouseDragged(MouseEvent e) {
        }
        @Override
        public void mousePressed(MouseEvent e) {
            pressed = true;
            repaint();
        }
        @Override
        public void mouseReleased(MouseEvent e) {
            pressed = false;
        }
        @Override
        public void mouseEntered(MouseEvent e) {}

    }
    public class Tools extends JPanel{
        EditorPanel editor;
        public Tools(EditorPanel editor){
            super(new FlowLayout());
            this.editor = editor;
            this.setOpaque(true);

            ImageIcon pencil = new ImageIcon(getClass().getResource("/tools/pencil.png"));
            ImageIcon eraser = new ImageIcon(getClass().getResource("/tools/eraser.png"));

            Image pencilImage = pencil.getImage().getScaledInstance(50, 50, Image.SCALE_SMOOTH);
            ImageIcon resizedPencil = new ImageIcon(pencilImage);

            Image eraserImage = eraser.getImage().getScaledInstance(50, 50, Image.SCALE_SMOOTH);
            ImageIcon resizedEraser = new ImageIcon(eraserImage);

            JButton p = new JButton(resizedPencil);
            JButton e = new JButton(resizedEraser);

            p.setPreferredSize(new Dimension(50, 50));
            e.setPreferredSize(new Dimension(50, 50));

            p.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    editor.action = "Draw";
                }
            });
            e.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    editor.action = "Delete";
                }
            });

            JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));

            buttonPanel.add(p);
            buttonPanel.add(e);

            add(buttonPanel, BorderLayout.NORTH);
        }
    }

    public class TexturePanel extends JPanel implements MouseListener{
        private List<String> textureNames = new ArrayList<>();
        private List<JLabel> labels = new ArrayList<>();
        private EditorPanel editor;
        private int flag1 = 0;
        private int flag2 = 0;
        private JLabel lastHoveredLabel = null;

        public TexturePanel(EditorPanel editor){
            super(new FlowLayout());
            this.editor = editor;
            this.setOpaque(true);
            addMouseListener(this);
            paintTextures();
        }

        public void paintTextures(){
            String[] folders = {"textures/", "objects/", "playerModel/"};
            for (String path : folders){
                File[] textures = new File("/home/sandrai/Desktop/JAVA/Project/prachgle/Engine/src/main/resources/" + path).listFiles();
                for (File texture : textures){
                    String textureName = texture.getName();
                    if (!textureNames.contains(textureName)){
                        textureNames.add(textureName);
                        try{
                            BufferedImage img = ImageIO.read(texture);
                            int width = img.getWidth() * 5;
                            int height = img.getHeight() * 5;
                            Image scaled = img.getScaledInstance(width, height, Image.SCALE_SMOOTH);
                            ImageIcon icon = new ImageIcon(scaled, textureName);
                            JLabel label = new JLabel(icon);
                            label.addMouseListener(this);
                            labels.add(label);
                            this.add(label);
                        } catch (IOException e) {
                            throw new RuntimeException(e);
                        }
                        if (flag1 == 1) flag2++;
                    }
                    if (path.equalsIgnoreCase("playerModel/")) {
                        break;
                    }
                }
                flag1 = 1;
            }
        }
        public void paintComponent(Graphics g){
            super.paintComponent(g);
            revalidate();
        }

        @Override
        public void mouseClicked(MouseEvent e) {
            Component parent = e.getComponent().getParent();
            if (parent instanceof TexturePanel) {
                TexturePanel panel = (TexturePanel) parent;
                Component[] components = panel.getComponents();
                int i = 1;
                for (Component component : components) {
                    if (component instanceof JLabel && e.getComponent() == component) {
                        JLabel label = (JLabel) component;
                        ImageIcon icon = (ImageIcon) label.getIcon();
                        editor.tile = icon;
                        editor.number = i;
                        editor.player = false;
                        editor.object = false;
                        if (components[i-1] == components[(components.length)-1]) {
                            editor.player = true;
                            editor.playerID = i;
                            editor.playerImage = icon;
                        }
                        else if (i >= flag2 || i < components.length-1){
                            editor.object = true;
                        }
                    }
                    i++;
                }
            }
        }
        @Override
        public void mouseEntered(MouseEvent e) {
            if (e.getSource() instanceof JLabel) {
                JLabel label = (JLabel) e.getSource();
                lastHoveredLabel = label;
                label.setBorder(BorderFactory.createLineBorder(Color.BLACK, 2));
                label.setCursor(new Cursor(Cursor.HAND_CURSOR));
            }
        }

        @Override
        public void mouseExited(MouseEvent e) {
            if (e.getSource() instanceof JLabel) {
                JLabel label = (JLabel) e.getSource();
                if (label == lastHoveredLabel) {
                    label.setBorder(null);
                    label.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
                    lastHoveredLabel = null;
                }
            }
        }
        @Override
        public void mousePressed(MouseEvent e) {

        }

        @Override
        public void mouseReleased(MouseEvent e) {

        }

    }
}
