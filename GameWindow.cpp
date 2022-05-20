#include "GameWindow.hpp"

#include <random>
#include <iostream>

GameWindow::GameWindow() {
    // ---- Main window ----
    window.create(sf::VideoMode(800, 600), "Doppelkopf");
    window.setFramerateLimit(60);
}

void GameWindow::shuffleCards(std::vector<Card *> &availableCards) {
    std::shuffle(availableCards.begin(), availableCards.end(), std::mt19937(std::random_device()()));
}

void GameWindow::init() {

    // ---- Preloading textures ----
    Card::loadTextures();

    // ---- Game objects ----
    std::vector<Card *> availableCards;
    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        cards[i] = Card(i);
        availableCards.push_back(&cards[i]);
    }
    shuffleCards(availableCards);

    for (int i = 0; i < 4; ++i) {
        std::vector<Card *> playerHand(
                make_move_iterator(availableCards.begin() + i * NUMBER_OF_CARDS_PER_PLAYER),
                make_move_iterator(
                        availableCards.begin() + NUMBER_OF_CARDS_PER_PLAYER + i * NUMBER_OF_CARDS_PER_PLAYER));
        players[i] = Player(playerHand);
        players[i].placeInQueue = i;
    }

    // ---- Game state ----
    currentTrick.clear();
    currentTrickCard = nullptr;
    playedTricks = 0;
    currentPlayer = 0;
}

void GameWindow::update() {

    if (sf::Event event; window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                Card *card = players[currentPlayer].playClickedCard(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                if (card != nullptr) {
                    currentTrick.push_back(card);
                    if (!currentTrickCard || doesTrick(currentTrickCard, card)) {
                        currentTrickCard = card;
                        currentTrickHolder = currentPlayer;
                    }

                    if (currentTrick.size() == 4) {
                        std::swap(currentTrick.front(), currentTrick[currentTrickHolder]);
                        players[currentTrickHolder].addTrick(currentTrick);
                        currentTrick.clear();

                        currentTrickCard = nullptr;
                        playedTricks++;
                    }


                    beginnNextPlayersTurn();
                }
            }
        }
    }
}

void GameWindow::render() {
    window.clear();

    for (int i = 0; i < 4; ++i) {
        window.draw(players[i]);

    }

    if (!currentTrick.empty()) {
        Card::arrangeAsStack(currentTrick, sf::Vector2f (WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2), sf::Vector2f(WINDOW_WIDTH / 16, 0), 0);
    }

    for (auto &card: currentTrick) {
        card->makeVisible();
        window.draw(*card);
    }

    window.display();

}

void GameWindow::beginnNextPlayersTurn() {
    currentPlayer = (currentPlayer + 1) % NUMBER_OF_PLAYERS;
    for (auto &player: players) {
        player.placeInQueue = ((player.placeInQueue == 0) ? NUMBER_OF_PLAYERS : player.placeInQueue) - 1;
    }
}

void GameWindow::close() {
    window.close();
}

bool GameWindow::doesTrick(Card *oldCard, Card *newCard) {
    return oldCard->trumpValue > 0 && newCard->trumpValue > oldCard->trumpValue ||  // both trump
           (oldCard->trumpValue < 0 && newCard->trumpValue > 0) ||  // old card is not trump and new card is trump
           (newCard->color == oldCard->color && newCard->trumpValue > oldCard->trumpValue); // same color and not trump

}
