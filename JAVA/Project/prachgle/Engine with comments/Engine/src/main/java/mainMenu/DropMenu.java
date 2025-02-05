package mainMenu;

import javafx.geometry.Pos;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuButton;
import javafx.scene.control.MenuItem;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;



public class DropMenu {
    private final MenuButton menuButton;
    private final Menu menu;

    public DropMenu(String buttonText, MenuItem... menuItems) {
        menuButton = new MenuButton(buttonText);
        menu = new Menu();

        // добавляем элементы в меню
        menu.getItems().addAll(menuItems);

        // добавляем меню в кнопку
        menuButton.getItems().add(menu);

        // добавляем обработчик для открытия меню при нажатии на кнопку
        menuButton.addEventHandler(MouseEvent.MOUSE_CLICKED, e -> menu.show());
    }

    // метод для установки горизонтальной ориентации меню
    public void setHorizontal() {
        menuButton.getStyleClass().add("split-button-horizontal");
        menuButton.setPrefWidth(120);
    }

    // метод для установки вертикальной ориентации меню
    public void setVertical() {
        menuButton.getStyleClass().add("split-button-vertical");
        menuButton.setPrefHeight(120);
    }

    // метод для установки выравнивания кнопки в контейнере
    public void setAlignment(HBox hbox, Pos position) {
        hbox.getChildren().add(menuButton);
        hbox.setAlignment(position);
    }

    // метод для установки выравнивания кнопки в контейнере
    public void setAlignment(VBox vbox, Pos position) {
        vbox.getChildren().add(menuButton);
        vbox.setAlignment(position);
    }

    // метод для получения кнопки
    public MenuButton getMenuButton() {
        return menuButton;
    }

    // метод для получения меню
    public Menu getMenu() {
        return menu;
    }
}
