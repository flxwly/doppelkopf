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

bool Player::isPlayable(std::vector<std::pair<Card *, int>> currentTrick, Card *card) {
    if (card == nullptr) {
        return false;
    }

    if (currentTrick.empty()) {
        return true;
    }

    Card *firstCard = currentTrick.front().first;

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
        availableCards.erase(std::remove(availableCards.begin(), availableCards.end(), trick[i]));
    }
    allTricks.emplace_back(moreInfoTrick, trickHolder);

    std::cout << "Player " << playerID << " got: " << evaluateState(allTricks)[playerID] << " num of available cards: "
              << availableCards.size() << std::endl;
}

std::array<int, 4>
Player::evaluateState(const std::vector<std::pair<std::vector<std::pair<Card *, int>>, int>> &allTricks) {

    std::array<int, 4> points = {0, 0, 0, 0};
    for (const auto &trick: allTricks) {
        points[trick.second] += std::accumulate(trick.first.begin(), trick.first.end(), 0,
                                                [](int i, const std::pair<Card *, int> &c) {
                                                    return c.first->pointValue + i;
                                                });
    }

    return points;
}

Card *Player::maxN(int maxDepth, int depth, std::vector<Card *> availableCards, std::vector<Card *> playerCards,
                   std::pair<std::vector<std::pair<Card *, int>>, int> currentTrick, int currentPlayer,
                   std::vector<std::pair<std::vector<std::pair<Card *, int>>, int>> allTricks) {
    if (depth == maxDepth) {
        return nullptr;
    }



    if (currentPlayer == playerID) {
        Card *bestCard = nullptr;
        std::array<int, 4> bestEval = {0, 0, 0, 0};

        for (int i = 0; i < playerCards.size(); i++) {
            auto c = playerCards[i];
            if (!isPlayable(currentTrick.first, c))
                continue;

            if (Card::doesTrick(currentTrick.first[currentTrick.second].first, c)) {
                currentTrick.second = playerID;
            }

            auto newPlayerCards = playerCards;
            newPlayerCards.erase(newPlayerCards.begin() + i);
            currentTrick.first.emplace_back(c, playerID);

            if (currentTrick.first.size() == 4) {
                allTricks.emplace_back(currentTrick);

                // TODO do something with return value
                maxN(maxDepth, depth + 1, availableCards, newPlayerCards, {}, currentTrick.second, allTricks);
            } else {

                // Call maxN with next player and updated card
                maxN(maxDepth, depth + 1, availableCards, newPlayerCards, currentTrick, (currentPlayer + 1 > 3) ? 0 : currentPlayer + 1, allTricks);
            }
        }
    } else {
        for (int i = 0; i < availableCards.size(); i++) {
            auto c = availableCards[i];
            if (!isPlayable(currentTrick.first, c))
                continue;

            if (Card::doesTrick(currentTrick.first[currentTrick.second].first, c))
                currentTrick.second = playerID;

            auto newAvailableCards = availableCards;
            newAvailableCards.erase(newAvailableCards.begin() + i);
            currentTrick.first.emplace_back(c, playerID);

            if (currentTrick.first.size() == 4) {
                allTricks.emplace_back(currentTrick);

                // TODO do something with return value
                maxN(maxDepth, depth + 1, availableCards, playerCards, {}, currentTrick.second, allTricks);
            } else {

                // Call maxN with next player and updated card
                maxN(maxDepth, depth + 1, availableCards, playerCards, currentTrick, (currentPlayer + 1 > 3) ? 0 : currentPlayer + 1, allTricks);
            }
        }
    }


    return nullptr;
}
