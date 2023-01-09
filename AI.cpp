#include "AI.hpp"
#include "Player.hpp"

Card *AI::chooseCard(std::vector<Card *> currentTrick) {

    std::vector<Card *> playerCards = player->cards;
//    Card * card = nullptr;
//    do {
//        card = playerCards[rand() % playerCards.size()];
//    }
//    while(!isPlayable(currentTrick, card));
    return playerCards.back();
}

bool AI::isPlayable(std::vector<Card *> currentTrick, Card* card) {
    if (card == nullptr) {
        return false;
    }

    if (currentTrick.empty()) {
        return true;
    }

    std::vector<Card *> cards = player->cards;
    Card* firstCard = currentTrick.front();

    // first Card of trick is trump
    if (firstCard->trumpValue > 0) {
        bool playerHasTrump = std::any_of(cards.begin(), cards.end(), [](Card *c) {
            return c->trumpValue > 0;
        });

        return playerHasTrump == card->trumpValue > 0;
    } else {
        bool playerHasSameColor = std::any_of(cards.begin(), cards.end(), [firstCard](Card *c) {
            return c->trumpValue < 0 && c->color == firstCard->color;
        });


        // Hat selbe Farbe -> card.color == firstcard.color && card kein trumpf
        // Hat nicht selbe Farbe -> card.color egal
        return !playerHasSameColor || (playerHasSameColor && (card->color != firstCard->color && card->trumpValue < 0));
    }
}
