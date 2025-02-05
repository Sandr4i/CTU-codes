package mainMenu;

import javafx.application.Platform;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.stage.Modality;
import javafx.stage.Stage;

public class ExitButton{
    public static void showAndWait() {
        Stage confirmStage = new Stage();
        confirmStage.setTitle("Confirmation");
        confirmStage.initModality(Modality.APPLICATION_MODAL);

        VBox confirmBox = new VBox();
        confirmBox.setAlignment(Pos.CENTER);
        confirmBox.setSpacing(10);

        Label confirmLabel = new Label("Are you sure?");
        confirmLabel.setFont(new Font(20));
        confirmBox.getChildren().add(confirmLabel);

        HBox confirmButtons = new HBox();
        confirmButtons.setAlignment(Pos.CENTER);
        confirmButtons.setSpacing(10);

        Button yesButton = new Button("Yes");
        yesButton.setOnAction(event1 -> {
            confirmStage.close();
            Platform.exit();
        });

        Button noButton = new Button("No");
        noButton.setOnAction(event1 -> confirmStage.close());

        confirmButtons.getChildren().addAll(yesButton, noButton);
        confirmBox.getChildren().add(confirmButtons);

        Scene confirmScene = new Scene(confirmBox, 400, 150);
        confirmStage.setScene(confirmScene);
        confirmStage.showAndWait();
    }
}
