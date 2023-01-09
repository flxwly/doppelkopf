#ifndef DOPPELKOPF_PLAYER_HPP
#define DOPPELKOPF_PLAYER_HPP


#include <list>
#include <cmath>
#include <random>
#include <utility>
#include "Card.hpp"
#include "WindowSettings.hpp"

class Player {
public:
    Player() = default;

    explicit Player(std::vector<Card *> cards, int playerID) {
        this->cards = std::move(cards);
        this->playerID = playerID;
    };

    Card *playCard(std::vector<Card *> currentTrick, Card *clickedCard);
    Card *playCard(std::vector<Card *> currentTrick);
    bool isPlayable(std::vector<Card *> currentTrick, Card* card);

    void addTrick(const std::vector<Card *> &trick, int currentPlayer, int trickHolder);

    Card *chooseCard(std::vector<Card *> currentTrick);

    std::array<int, 4> evaluateState();

    int playerID = 0;
    int placeInQueue = 0;
    std::vector<Card *> cards;
    std::vector<std::vector<Card *>> tricks;


    // AI Part
    std::vector<std::pair<std::vector<std::pair<Card *, int>>, int>> allTricks;
};


#endif //DOPPELKOPF_PLAYER_HPP
