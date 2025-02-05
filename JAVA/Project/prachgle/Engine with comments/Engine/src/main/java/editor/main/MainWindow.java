package editor.main;


import java.awt.event.*;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import javax.swing.*;

import mainMenu.MakeYourGameButton;

public class MainWindow extends JFrame implements ActionListener {

    private JDesktopPane pane;
    public CreateMap frame;
    private int[][] map;
    public MainWindow(){
        super("Map editor");
        this.pane = new JDesktopPane();
        this.setContentPane(pane);
        this.setExtendedState(this.getExtendedState()|JFrame.MAXIMIZED_BOTH);
        pane.setDragMode(JDesktopPane.OUTLINE_DRAG_MODE);
        this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        menu();
    }
    public void menu(){
        JMenuBar bar = new JMenuBar();
        JMenu file = new JMenu("File");
        JMenuItem newItem = new JMenuItem("New file");
        JMenuItem exportItem = new JMenuItem("Export as text file");
        JMenuItem exit = new JMenuItem("Exit");

        newItem.addActionListener(this);
        exit.addActionListener(this);
        exportItem.addActionListener(this);

        file.add(newItem);
        file.add(exportItem);
        file.add(exit);

        bar.add(file);
        this.setJMenuBar(bar);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String command = e.getActionCommand();
        if (command.equalsIgnoreCase("New file")){
            frame = new CreateMap(this.getContentPane());
            frame.setVisible(true);
            this.pane.add(frame, 0);
        }
        else if (command.equalsIgnoreCase("Export as text file")){
//            MakeYourGameButton.f.dispose();
            map = frame.frame.editorPanel.grid;
            int flag = 0;
            System.out.println("Map length: " + map.length + " " + map[0].length);
            for (int i = 0; i < map.length; i++) {
                for (int j = 0; j < map[0].length; j++) {
                    if (map[i][j] == 0){
                        System.out.println("coords are: i :" + i + " j: " + j + " " + map[j][i]);
                        flag = 1;
                        break;
                    }
                }
            }
            if (flag == 1){
                Object[] options = { "Close" };
                JOptionPane.showOptionDialog(null, "You need to fill all the blocks!", "Attention!",
                        JOptionPane.DEFAULT_OPTION, JOptionPane.INFORMATION_MESSAGE, null, options, options[0]);
            }
            else {
                String path = frame.path + ".txt";
                System.out.println("Path: " + path);
                try (BufferedWriter writer = new BufferedWriter(new FileWriter(path))) {
                    for (int i = 0; i < map.length; i++) {
                        for (int j = 0; j < map[0].length; j++) {
                            writer.write(Integer.toString(map[i][j]));
                            writer.write(" ");
                        }
                        writer.newLine();
                    }
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
                Object[] options = { "Close" };
                int result = JOptionPane.showOptionDialog(null, "Map was saved", "Success",
                        JOptionPane.DEFAULT_OPTION, JOptionPane.INFORMATION_MESSAGE, null, options, options[0]);
                if (result == 0) {
                    frame.frame.dispose();
                    frame.dispose();
                    this.dispose();
                }
            }
        }
        else{
          MakeYourGameButton.f.dispose();
        }
    }
}
