#ifndef DOPPELKOPF_PLAYER_HPP
#define DOPPELKOPF_PLAYER_HPP


#include <list>
#include "Card.hpp"
#include "WindowSettings.hpp"

class Player : public sf::Drawable {
public:
    Player() = default;

    explicit Player(std::vector<Card *> cards);

    Card* playClickedCard(sf::Vector2f mousePosition);
    void addTrick(const std::vector<Card *>& trick) { tricks.push_back(trick); };

    int placeInQueue = 0;

private:
    std::vector<Card *> cards;
    std::vector<std::vector<Card *>> tricks;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //DOPPELKOPF_PLAYER_HPP
