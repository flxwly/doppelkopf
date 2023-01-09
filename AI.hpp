#ifndef DOPPELKOPF_AI_HPP
#define DOPPELKOPF_AI_HPP

#include <array>
#include "Card.hpp"

class Player;

class AI {
public:
    AI() = default;
    explicit AI(Player *player) { this->player = player; }

    Card *chooseCard(std::vector<Card *> currentTrick);
    bool isPlayable(std::vector<Card *> currentTrick, Card* card);


    // Gamestate
    Player *player;
    std::vector<Card *> availableCards;
    std::vector<std::array<std::pair<Card *, int>, 4>> playerTricks;

};


#endif //DOPPELKOPF_AI_HPP
