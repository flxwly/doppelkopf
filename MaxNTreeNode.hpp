#ifndef DOPPELKOPF_MAXNTREENODE_HPP
#define DOPPELKOPF_MAXNTREENODE_HPP

#include <utility>
#include <vector>
#include <numeric>
#include "Card.hpp"

class MaxNTreeNode {
public:
    MaxNTreeNode() = default;
    MaxNTreeNode(std::vector<Card *> availableCards,
                 std::vector<Card *> playerCards,
                 std::pair<std::vector<std::pair<Card *, int>>, int> currentTrick,
                 std::vector<std::pair<std::vector<std::pair<Card *, int>>, int>> allTricks,
                 int currentPlayer,
                 int playerID,
                 Card *playedCard);

    std::pair<std::array<int, 4>, Card*> specMax(std::array<int, 4> pScore, std::array<int, 4> gpScore);
    void generateTree(int maxDepth, int depth);
    std::array<int, 4> evaluateState();
    Card* playedCard;

    std::vector<Card *> availableCards;
    std::vector<Card *> playerCards;
    std::pair<std::vector<std::pair<Card *, int>>, int> currentTrick;
    std::vector<std::pair<std::vector<std::pair<Card *, int>>, int>> allTricks;
    int currentPlayer;
    int playerID;

    MaxNTreeNode *parent;
    std::vector<MaxNTreeNode> childNodes;


};


#endif //DOPPELKOPF_MAXNTREENODE_HPP
