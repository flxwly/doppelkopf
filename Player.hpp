#ifndef DOPPELKOPF_PLAYER_HPP
#define DOPPELKOPF_PLAYER_HPP


#include <list>
#include <cmath>
#include <random>
#include <utility>
#include "Card.hpp"
#include "WindowSettings.hpp"
#include "MaxNTreeNode.hpp"

class Player {
public:
    Player() = default;

    explicit Player(const std::vector<Card *>& cards, int playerID, std::vector<Card *> allCards) {
        this->cards = cards;
        this->playerID = playerID;
        for (auto c: this->cards) {
            allCards.erase(std::remove(allCards.begin(), allCards.end(), c), allCards.end());
        }
        availableCards = allCards;
        gameState = MaxNTreeNode(availableCards, cards, {}, {}, 0, playerID, nullptr);
    };

    Card *playCard(const std::pair<std::vector<std::pair<Card *, int>>, int> &currentTrick, Card *clickedCard);

    Card *playCard(const std::pair<std::vector<std::pair<Card *, int>>, int> &currentTrick);

    void removeAvailableCard(Card * c) {availableCards.erase(std::remove(availableCards.begin(), availableCards.end(), c), availableCards.end());};

    void addTrick(const std::vector<std::pair<Card *, int>> &trick, int currentPlayer, int trickHolder);

    Card *chooseCard(const std::pair<std::vector<std::pair<Card *, int>>, int> &currentTrick);

    int playerID = 0;
    int placeInQueue = 0;

    std::vector<Card *> cards;
    std::vector<std::vector<std::pair<Card *, int>>> tricks;
    // AI Part
    MaxNTreeNode gameState;

    std::vector<Card *> availableCards;
    std::vector<std::pair<std::vector<std::pair<Card *, int>>, int>> allTricks;
};


#endif //DOPPELKOPF_PLAYER_HPP
