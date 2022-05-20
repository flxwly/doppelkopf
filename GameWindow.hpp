#ifndef DOPPELKOPF_GAMEWINDOW_HPP
#define DOPPELKOPF_GAMEWINDOW_HPP

#include <SFML/Graphics.hpp>
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

    static void shuffleCards(std::vector<Card *> &availableCards);
    static bool doesTrick(Card *oldCard, Card *newCard);

    explicit operator bool() const { return window.isOpen(); }

private:
    // ---- Main window ----
    sf::RenderWindow window;

    // ---- Game objects ----
    std::array<Card, 52> cards;
    std::array<Player, 4> players;

    // ---- Game state ----
    std::vector<Card *> currentTrick;
    Card *currentTrickCard = nullptr;
    int playedTricks = 0;
    int currentTrickHolder = 0;
    int currentPlayer = 0;
};

#endif //DOPPELKOPF_GAMEWINDOW_HPP
