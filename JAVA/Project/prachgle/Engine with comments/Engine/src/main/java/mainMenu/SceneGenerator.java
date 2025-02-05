package mainMenu;

import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.input.KeyEvent;

public interface SceneGenerator {
    Scene generateScene();

    EventHandler<KeyEvent> getKeyHandler();
}
