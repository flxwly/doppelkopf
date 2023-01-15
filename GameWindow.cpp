#include "GameWindow.hpp"

#include <random>
#include <iostream>

GameWindow::GameWindow() {
    // ---- Main window ----
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Doppelkopf");
    window.setFramerateLimit(60);

    if (!gameFont.loadFromFile("../resources/fonts/arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        exit(999);
    }
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

    // ---- Players ----
    for (int i = 0; i < 4; ++i) {
        std::vector<Card *> playerHand(
                make_move_iterator(availableCards.begin() + i * NUMBER_OF_CARDS_PER_PLAYER),
                make_move_iterator(
                        availableCards.begin() + NUMBER_OF_CARDS_PER_PLAYER + i * NUMBER_OF_CARDS_PER_PLAYER));
        players[i] = Player(playerHand, i, availableCards);
    }

    // ---- Game state ----
    currentTrick.first.clear();
    currentTrick.second = currentPlayer;
    playedTricks = 0;
    currentPlayer = 0;
}

void GameWindow::update() {

    if (sf::Event event; window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                close();
                break;

            case sf::Event::MouseMoved:
                for (auto &c: cards) {
                    if (c.getShape().getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                        c.highlight();
                    else
                        c.deHighlight();
                }
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    Card *card;

                    if (currentTrick.first.size() == 4) {
                        for (auto &player: players) {
                            player.addTrick(currentTrick.first, currentPlayer, currentTrick.first[currentTrick.second].second);
                        }

                        playedTricks++;

                        beginnNewTrick();
                    }

                    if (currentPlayer == HUMAN_PLAYER) {
                        Card *clickedCard = nullptr;
                        for (const auto &c: players[currentPlayer].cards) {
                            if (c->getShape().getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                                clickedCard = c;
                                break;
                            }
                        }
                        card = players[currentPlayer].playCard(currentTrick, clickedCard);
                    } else {
                        card = players[currentPlayer].playCard(currentTrick);
                    }

                    if (card) {
                        if (currentTrick.first.empty() || Card::doesTrick(currentTrick.first[currentTrick.second].first, card)) {
                            currentTrick.second = currentTrick.first.size();
                        }

                        currentTrick.first.emplace_back(card, currentPlayer);

                        if (currentTrick.first.size() != 4) {
                            beginnNextPlayersTurn();
                        }

                        for (int i = 0; i < NUMBER_OF_PLAYERS; ++i) {
                            if (currentPlayer == i)
                                continue;

                            players[i].removeAvailableCard(card);
                        }
                    }
                }
        }
    }
}

void GameWindow::render() {
    window.clear();

    Card::arrangeAsHand(players[0].cards);
    for (auto c: players[0].cards) {
        c->makeVisible();
        window.draw(*c);
    }


    if (!currentTrick.first.empty()) {
        Card::arrangeAsStack(currentTrick.first, sf::Vector2f(WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2),
                             sf::Vector2f(WINDOW_WIDTH / 16, 0), 0);
    }

    for (auto &card: currentTrick.first) {
        card.first->makeVisible();
        window.draw(*card.first);
    }

    sf::Text whosTurn;
    whosTurn.setFont(gameFont);
    whosTurn.setString("Player " + std::to_string(currentPlayer));
    whosTurn.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3);
    window.draw(whosTurn);

    window.display();

}

void GameWindow::beginnNextPlayersTurn() {
    currentPlayer = (currentPlayer + 1) % NUMBER_OF_PLAYERS;
    for (auto &player: players) {
        player.placeInQueue = ((player.placeInQueue == 0) ? NUMBER_OF_PLAYERS : player.placeInQueue) - 1;
    }
}

void GameWindow::beginnNewTrick() {

    int currentTrickHolder = currentTrick.first[currentTrick.second].second;


    for (auto &player: players) {
        player.placeInQueue = (player.playerID - currentTrickHolder < 0) ? (player.playerID - currentTrickHolder) +
                                                                           NUMBER_OF_PLAYERS : (player.playerID -
                                                                                                currentTrickHolder);
    }

    currentPlayer = currentTrickHolder;

    currentTrick.first.clear();
    currentTrick.second = 0;
}

void GameWindow::close() {
    window.close();
}
