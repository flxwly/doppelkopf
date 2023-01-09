#ifndef DOPPELKOPF_PLAYER_HPP
#define DOPPELKOPF_PLAYER_HPP


#include <list>
#include <cmath>
#include <random>
#include <utility>
#include "Card.hpp"
#include "WindowSettings.hpp"
#include "AI.hpp"

class Player {
public:
    Player() = default;

    explicit Player(std::vector<Card *> cards, int playerID) {
        this->cards = std::move(cards);
        ai = AI(this);
        this->playerID = playerID;
        this->placeInQueue = playerID;
    };

    Card *chooseCard(std::vector<Card *> currentTrick);

    Card *playCard(std::vector<Card *> currentTrick, Card *clickedCard);

    Card *playCard(std::vector<Card *> currentTrick);

    void addTrick(const std::vector<Card *> &trick) { tricks.push_back(trick); };

    int playerID = 0;
    int placeInQueue = 0;
    std::vector<std::vector<Card *>> tricks;
    std::vector<Card *> cards;

    AI ai;
};


#endif //DOPPELKOPF_PLAYER_HPP
