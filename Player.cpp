#include "Player.hpp"

#include <random>
#include <iostream>

Card *Player::playCard(const std::pair<std::vector<std::pair<Card *, int>>, int> &currentTrick, Card *card) {

    if (Card::isPlayable(currentTrick.first, cards, card)) {
        cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());
        return card;
    }

    return nullptr;
}

Card *Player::playCard(const std::pair<std::vector<std::pair<Card *, int>>, int> &currentTrick) {
    return playCard(currentTrick, chooseCard(currentTrick));
}

Card *Player::chooseCard(const std::pair<std::vector<std::pair<Card *, int>>, int> &currentTrick) {

    gameState = MaxNTreeNode(availableCards, cards, {currentTrick, }, allTricks, playerID, playerID, nullptr);
    gameState.generateTree(8, 0);

    Card* bestMove = gameState.childNodes.front().playedCard;
    auto largestElement = gameState.childNodes.front().evaluateState();
    for (int i = 1; i < gameState.childNodes.size(); i++) {
        auto eval = gameState.childNodes[i].evaluateState();
        if (eval[playerID] > largestElement[playerID]) {
            largestElement = eval;
            bestMove = gameState.childNodes.front().playedCard;
        }
    }
    return bestMove;
}

void Player::addTrick(const std::vector<std::pair<Card *, int>> &trick, int currentPlayer, int trickHolder) {
    if (playerID == trickHolder)
        tricks.push_back(trick);
    allTricks.emplace_back(trick, trickHolder);
}