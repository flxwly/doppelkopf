#include <iostream>
#include "GameWindow.hpp"

int main() {
    GameWindow gameWindow = GameWindow();
    gameWindow.init();

    while (gameWindow) {
        gameWindow.update();
        gameWindow.render();
    }
}
