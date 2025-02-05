package mainMenu;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.input.KeyEvent;
import javafx.stage.Stage;

public class MainMenu extends Application {
    @Override
    public void start(Stage stage) {
        SceneGenerator sceneGenerator = new SceneGenerate();

        Scene scene = sceneGenerator.generateScene();
        stage.setTitle("Game Builder ver. 0.2");
        if (sceneGenerator.getKeyHandler() != null) {
            stage.addEventHandler(KeyEvent.KEY_PRESSED, sceneGenerator.getKeyHandler());
        }
        stage.setScene(scene);
        stage.setMaximized(true);
        stage.show();

    }
    public static void main(String[] args){launch(args);}
}
