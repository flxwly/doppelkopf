#include <random>
#include <iostream>
#include "Player.hpp"

Card *Player::playCard(std::vector<Card *> currentTrick, Card *card) {

    if (isPlayable(currentTrick, card)) {
        cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());
        return card;
    }

    return nullptr;
}

Card *Player::playCard(std::vector<Card *> currentTrick) {
    return playCard(currentTrick, chooseCard(currentTrick));
}

bool Player::isPlayable(std::vector<Card *> currentTrick, Card *card) {

    // Check if card is playable
    // If there's no first card, the player can play any card
    // If the player can play a card of the same color as the first card, he must play it. If not, he must play any card
    // Colors are E, R, G, S and T

    if (card == nullptr) {
        return false;
    }

    if (currentTrick.empty()) {
        return true;
    }

    Card *firstCard = currentTrick.front();

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
        return (playerHasSameColor && card->color == firstCard->color && card->trumpValue < 0) || !playerHasSameColor;
    }
}

Card *Player::chooseCard(std::vector<Card *> currentTrick) {
    for (auto &card: cards) {
        if (isPlayable(currentTrick, card)) {
            return card;
        }
    }
    return nullptr;
}

void Player::addTrick(const std::vector<Card *> &trick, int currentPlayer, int trickHolder) {
    if (playerID == trickHolder)
        tricks.push_back(trick);

    std::vector<std::pair<Card *, int>> moreInfoTrick;
    for (int i = trick.size() - 1; i >= 0; --i) {
        int player = (currentPlayer - i >= 0) ? currentPlayer - i : currentPlayer - i + NUMBER_OF_PLAYERS;
        moreInfoTrick.emplace_back(trick[i], player);
    }
    allTricks.emplace_back(moreInfoTrick, trickHolder);

    std::cout << "Player " << playerID << " got: " << evaluateState()[playerID] << std::endl;
}

std::array<int, 4> Player::evaluateState() {

    std::array<int, 4> points = {0, 0, 0, 0};
    for (auto trick: allTricks) {
        points[trick.second] += std::accumulate(trick.first.begin(), trick.first.end(), 0, [](int i, const std::pair<Card*, int> &c) {return c.first->pointValue + i;});
    }

    return points;
}
