#include "MaxNTreeNode.hpp"


void MaxNTreeNode::generateTree(int maxDepth, int depth) {

    if (depth == maxDepth) {
        return;
    }

    if (currentPlayer == playerID) {
        for (int i = 0; i < playerCards.size(); i++) {
            auto c = playerCards[i];
            if (!Card::isPlayable(currentTrick.first, playerCards, c))
                continue;

            if (currentTrick.first.empty() || Card::doesTrick(currentTrick.first.at(currentTrick.second).first, c)) {
                currentTrick.second = currentTrick.first.size();
            }

            auto newPlayerCards = playerCards;
            newPlayerCards.erase(newPlayerCards.begin() + i);
            currentTrick.first.emplace_back(c, playerID);

            if (currentTrick.first.size() == 4) {
                allTricks.emplace_back(currentTrick);

                childNodes.push_back(
                        MaxNTreeNode(availableCards, newPlayerCards, {{}, 0}, allTricks, currentTrick.second, playerID, c));
            } else {
                childNodes.emplace_back(availableCards, newPlayerCards, currentTrick, allTricks,
                                        (currentPlayer + 1 > 3) ? 0 : currentPlayer + 1, playerID, c);
            }
            childNodes.back().generateTree(maxDepth, depth + 1);
        }
    } else {
        for (int i = 0; i < availableCards.size(); i++) {
            auto c = availableCards[i];
            if (!Card::isPlayable(currentTrick.first, {}, c))
                continue;

            if (currentTrick.first.empty() || Card::doesTrick(currentTrick.first[currentTrick.second].first, c))
                currentTrick.second = currentTrick.first.size();

            auto newAvailableCards = availableCards;
            newAvailableCards.erase(newAvailableCards.begin() + i);
            currentTrick.first.emplace_back(c, playerID);

            if (currentTrick.first.size() == 4) {
                allTricks.emplace_back(currentTrick);
                childNodes.push_back(
                        MaxNTreeNode(newAvailableCards, playerCards, {{}, 0}, allTricks, currentPlayer, playerID, c));
            } else {
                // Call maxN with next player and updated card
                childNodes.emplace_back(newAvailableCards, playerCards, currentTrick, allTricks,
                                        (currentPlayer + 1 > 3) ? 0 : currentPlayer + 1, playerID, c);
            }
            childNodes.back().generateTree(maxDepth, depth + 1);
        }
    }
}

std::array<int, 4> MaxNTreeNode::evaluateState() {

    if (childNodes.empty()) {
        std::array<int, 4> points = {0, 0, 0, 0};
        for (const auto &trick: allTricks) {
            points[trick.second] += std::accumulate(trick.first.begin(), trick.first.end(), 0,
                                                    [](int i, const std::pair<Card *, int> &c) {
                                                        return c.first->pointValue + i;
                                                    });
        }
        return points;
    } else {

        auto largestElement = childNodes.front().evaluateState();
        for (int i = 1; i < childNodes.size(); i++) {
            auto eval = childNodes[i].evaluateState();
            if (eval[playerID] > largestElement[playerID]) {
                largestElement = eval;
            }
        }
        return largestElement;
    }
}

MaxNTreeNode::MaxNTreeNode(std::vector<Card *> availableCards, std::vector<Card *> playerCards,
                           std::pair<std::vector<std::pair<Card *, int>>, int> currentTrick,
                           std::vector<std::pair<std::vector<std::pair<Card *, int>>, int>> allTricks,
                           int currentPlayer, int playerID, Card* playedCard) {

    this->availableCards = std::move(availableCards);
    this->playerCards = std::move(playerCards);
    this->currentTrick = currentTrick;
    this->allTricks = std::move(allTricks);
    this->currentPlayer = currentPlayer;
    this->playerID = playerID;
    this->playedCard = playedCard;
    this->parent = nullptr;

}
