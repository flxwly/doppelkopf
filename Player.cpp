#include "Player.hpp"


Player::Player(std::vector<Card *> cards) {
    this->cards = cards;
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    float i = 0;

    const float horizontalSpacingToWall = WINDOW_WIDTH / 16;
    const float verticalSpacingToWall = WINDOW_HEIGHT / 16;

    const float horizontalSpacing = WINDOW_WIDTH / (NUMBER_OF_CARDS_PER_PLAYER + 4);
    const float verticalSpacing =  WINDOW_HEIGHT / (NUMBER_OF_CARDS_PER_PLAYER + 4);

    sf::Vector2f startPos;
    sf::Vector2f spacing;
    float orientation;

    switch (placeInQueue) {
        case 0:
            startPos = sf::Vector2f(horizontalSpacingToWall, 15 * verticalSpacingToWall);
            spacing = sf::Vector2f(horizontalSpacing, 0);
            orientation = 0;
            break;
        case 1:
            startPos = sf::Vector2f(15 * horizontalSpacingToWall, 15 * verticalSpacingToWall);
            spacing = sf::Vector2f(0, -verticalSpacing);
            orientation = 270;
            break;
        case 2:
            startPos = sf::Vector2f(15 * horizontalSpacingToWall, verticalSpacingToWall);
            spacing = sf::Vector2f(-horizontalSpacing, 0);
            orientation = 180;
            break;
        case 3:
            startPos = sf::Vector2f(horizontalSpacingToWall, verticalSpacingToWall);
            spacing = sf::Vector2f(0, verticalSpacing);
            orientation = 90;
            break;
        default:
            return;
    }
    Card::arrangeAsStack(cards, startPos, spacing, orientation);
    for (auto card : cards) {

        if (placeInQueue == 0) {
            card->makeVisible();
        } else {
            card->makeInvisible();
        }

        target.draw(*card);
    }
}

Card *Player::playClickedCard(sf::Vector2f mousePosition) {
    Card *card = nullptr;
    for (auto c: cards) {
        if (c->getShape().getGlobalBounds().contains(mousePosition)) {
            card = c;
            break;
        }
    }
    if (card != nullptr) {
        cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());
    }

    return card;
}
