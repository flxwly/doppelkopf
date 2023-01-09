#include "Player.hpp"

#include <random>
#include <iostream>

Card *Player::playCard(std::vector<Card*> currentTrick, Card* card) {

    // Check if card is playable
    // If there's no first card, the player can play any card
    // If the player can play a card of the same color as the first card, he must play it. If not, he must play any card
    // Colors are E, R, G, S and T


    if (card == nullptr) {
        return nullptr;
    }

    if (currentTrick.empty()) {
        cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());
        return card;
    }

    Card* firstCard = currentTrick.front();

    if (firstCard->trumpValue > 0) {
        bool playerHasTrump = std::any_of(cards.begin(), cards.end(), [](Card *c) {
            return c->trumpValue > 0;
        });

        if (playerHasTrump && card->trumpValue < 0) {
            return nullptr;
        }
    } else {
        bool playerHasSameColor = std::any_of(cards.begin(), cards.end(), [firstCard](Card *c) {
            return c->trumpValue < 0 && c->color == firstCard->color;
        });
        if (playerHasSameColor && (card->color != firstCard->color || card->trumpValue > 0)) {
            return nullptr;
        }
    }

    cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());
    return card;
}

Card *Player::playCard(std::vector<Card*> currentTrick) {
    return playCard(currentTrick, ai.chooseCard(currentTrick));
}

Card *Player::chooseCard(std::vector<Card *> currentTrick) {
    std::shuffle(cards.begin(), cards.end(), std::mt19937(std::random_device()()));
    return cards.empty() ? nullptr : cards.front();
}
