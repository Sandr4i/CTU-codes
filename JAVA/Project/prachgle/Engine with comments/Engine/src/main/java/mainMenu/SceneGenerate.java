package mainMenu;

import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.image.Image;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.*;
import javafx.scene.text.Text;

import java.util.ArrayList;
import java.util.List;

public class SceneGenerate implements SceneGenerator {  // template from lessons
    private final List<Button> menuButtons = new ArrayList<>();
    private int width;
    private int height;
    private int coordX;
    private int coordY;
    private String path;
    @Override
    public Scene generateScene() {
        BorderPane borderPane = new BorderPane();

        BackgroundImage backgroundImage = new BackgroundImage(new Image("/backgrounds/im.jpg"),
                BackgroundRepeat.NO_REPEAT, BackgroundRepeat.NO_REPEAT, BackgroundPosition.DEFAULT,
                BackgroundSize.DEFAULT);
        Background background = new Background(backgroundImage);
        borderPane.setBackground(background);

        Text text = new Text();

        menuButtons.add(this.generateButton("Play Default Demo", text, borderPane));
        menuButtons.add(this.generateButton("Play Custom Demo", text, borderPane));
        menuButtons.add(this.generateButton("Make your game", text, borderPane));
        menuButtons.add(this.generateButton("About", text,borderPane));
        menuButtons.add(this.generateButton("Exit", text,borderPane));

        VBox centerBox = new VBox(); // vertical placement
        centerBox.setSpacing(10);
        centerBox.setAlignment(Pos.CENTER);
        centerBox.getChildren().addAll(menuButtons);

        borderPane.setCenter(centerBox);

        String css = getClass().getResource("/style/buttonStyle.css").toExternalForm(); //buttons
        borderPane.getStylesheets().add(css);

        return new Scene(borderPane);
    }
    @Override
    public EventHandler<KeyEvent> getKeyHandler() {
        return null;
    }

    private Button generateButton(String text, Text textToGenerate, BorderPane borderPane) {
        Button button = new Button(text);
        button.setOnAction(event -> {
                    switch (text) {
                        case "Exit":
                            ExitButton.showAndWait();
                            break;
                        case "About":
                            AboutButton.showAndWait();
                            break;
                        case "Play Default Demo":
                            PlayDemoDefaultButton demo = new PlayDemoDefaultButton();
                            demo.show();
                            break;
                        case "Play Custom Demo":
                            PlayDemoCustomButton demoCustom = new PlayDemoCustomButton();
                            demoCustom.worldWidth = width;
                            demoCustom.worldHeight = height;
                            demoCustom.coordX = coordX;
                            demoCustom.coordY = coordY;
                            demoCustom.path = path;
                            demoCustom.show();
                            break;
                        case "Make your game":
                            MakeYourGameButton make = new MakeYourGameButton();
                            make.showAndWait();
                            width = make.f.frame.worldWidth;
                            height = make.f.frame.worldHeight;
                            path = make.f.frame.path;
                            coordX = make.f.frame.frame.editorPanel.playerX;
                            coordY = make.f.frame.frame.editorPanel.playerY;

                            break;
                        default: //something
                            break;
                    }
                });
        button.setPrefSize(150, 50); //size of main buttons
        return button;
    }
}
