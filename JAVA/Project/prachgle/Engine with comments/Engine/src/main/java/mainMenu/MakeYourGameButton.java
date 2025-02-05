package mainMenu;

import editor.main.MainWindow;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.stage.FileChooser;
import javafx.stage.Modality;
import javafx.stage.Stage;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.List;

public class MakeYourGameButton {
    public static MainWindow f = new MainWindow();
    public static void showAndWait() {
        Stage editorStage = new Stage();
        editorStage.setTitle("Game Editor");
        editorStage.initModality(Modality.APPLICATION_MODAL);

        VBox confirmBox = new VBox();
        confirmBox.setAlignment(Pos.CENTER);
        confirmBox.setSpacing(10);

        HBox buttonBox = new HBox();
        buttonBox.setSpacing(10);

        Label confirmLabel = new Label("Choose your option");
        confirmLabel.setFont(new Font(20));
        confirmBox.getChildren().add(confirmLabel);

        VBox confirmButtons = new VBox();
        confirmButtons.setAlignment(Pos.CENTER);
        confirmButtons.setSpacing(10);

        List<MenuItem> menuItems = new ArrayList<>();
        menuItems.add(new MenuItem("Textures for players"));
        menuItems.add(new MenuItem("Textures for enemies"));
        menuItems.add(new MenuItem("Textures for objects"));
        menuItems.add(new MenuItem("Textures for textures"));

        Menu downloadMenu = new Menu("Download files");

        String[] directories = {"textures", "playerModel", "objects", "enemies"};

        for (int i = 0; i < directories.length; i++) {
            final int index = i;
            menuItems.get(i).setOnAction(actionEvent -> {
                FileChooser fileChooser = new FileChooser();
                fileChooser.setTitle("Select texture files");
                fileChooser.getExtensionFilters().addAll(new FileChooser.ExtensionFilter("Image Files", "*.png", "*.jpg", "*.gif"));
                fileChooser.setInitialDirectory(new File(System.getProperty("user.home")));
                List<File> selectedFiles = fileChooser.showOpenMultipleDialog(editorStage);
                if (selectedFiles != null) {
                    String destinationPath = System.getProperty("user.dir") + "/Engine/src/main/resources/" + directories[index];
                    Path destinationFilePath;

                    // Проверяем, существует ли папка и создаем ее при необходимости
                    File resourcesDir = new File(destinationPath);
                    if (!resourcesDir.exists()) {
                        try {
                            Files.createDirectory(resourcesDir.toPath());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }

                    for (File selectedFile : selectedFiles) {
                        String fileName = selectedFile.getName();
                        destinationFilePath = Paths.get(destinationPath, fileName);
                        try {
                            Files.copy(selectedFile.toPath(), destinationFilePath, StandardCopyOption.REPLACE_EXISTING);
                            System.out.println("File copied to: " + destinationFilePath.toAbsolutePath().toString());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            });
            downloadMenu.getItems().add(menuItems.get(i));
        }

        Button editorButton = new Button("Game Editor");
        editorButton.setOnAction(event1 -> {
            f.setVisible(true);
        });

        Button closeButton = new Button("Close");
        closeButton.setOnAction(event1 -> editorStage.close());

        confirmButtons.getChildren().addAll(new MenuButton("Download files", null, menuItems.toArray(new MenuItem[0])), editorButton , closeButton);
        confirmBox.getChildren().add(confirmButtons);

        Scene confirmScene = new Scene(confirmBox, 800, 300);
        editorStage.setScene(confirmScene);
        editorButton.setPrefSize(150, 50); //size of main buttons
        closeButton.setPrefSize(150, 50); //size of main buttons
        editorStage.showAndWait();

    }
}
