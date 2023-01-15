#ifndef DOPPELKOPF_GAMEWINDOW_HPP
#define DOPPELKOPF_GAMEWINDOW_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <list>
#include "Player.hpp"
#include "Card.hpp"
#include "WindowSettings.hpp"


class GameWindow {

public:
    GameWindow();

    void init();

    void update();

    void render();

    void close();

    void beginnNextPlayersTurn();
    void beginnNewTrick();

    static void shuffleCards(std::vector<Card *> &availableCards);

    explicit operator bool() const { return window.isOpen(); }

private:
    // ---- Main window ----
    sf::RenderWindow window;
    sf::Font gameFont;

    // ---- Game objects ----
    std::array<Card, 52> cards;
    std::array<Player, 4> players;

    // ---- Game state ----
    std::pair<std::vector<std::pair<Card *, int>>, int> currentTrick;
    int playedTricks = 0;
    int currentPlayer = 0;
};

#endif //DOPPELKOPF_GAMEWINDOW_HPP
