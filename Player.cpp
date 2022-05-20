#include "Player.hpp"


Player::Player(std::vector<Card *> cards) {
    this->cards = cards;
}


void Player::displayHand(sf::RenderTarget &target, sf::RenderStates states) const {

    const unsigned int count = cards.size();

    const float handHeight = WINDOW_HEIGHT / 16; // a
    const float handWidth = WINDOW_WIDTH / 2;

    const float x = (powf(handHeight, 2) + powf(handWidth/2, 2)) / (2 * handHeight);
    const float alpha = 2 * atan(2 * handHeight * handWidth / (powf(handHeight, 2) + powf(handWidth/2, 2)));
    const sf::Vector2f center = sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT + WINDOW_HEIGHT / 16 - x);


    for (int i = 0; i < count; ++i) {
        float a = 90 + alpha - alpha * (float) i / (float) count;
        sf::Vector2f pos = sf::Vector2f(cos(a) * x, sin(a) * x);
        cards[i]->setPosition(pos);
        cards[i]->setRotation(90 - a);
        target.draw(*cards[i]);
    }
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    float i = 0;

    const float horizontalSpacingToWall = WINDOW_WIDTH / 16;
    const float verticalSpacingToWall = WINDOW_HEIGHT / 16;

    const float horizontalSpacing = WINDOW_WIDTH / (NUMBER_OF_CARDS_PER_PLAYER + 4);
    const float verticalSpacing = WINDOW_HEIGHT / (NUMBER_OF_CARDS_PER_PLAYER + 4);

    sf::Vector2f startPos;
    sf::Vector2f spacing;
    float orientation;

    switch (placeInQueue) {
        case 0:
            displayHand(target, states);
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
    for (auto card: cards) {

        if (placeInQueue == 0) {
            card->makeVisible();
        } else {
            card->makeInvisible();
        }

        target.draw(*card);
    }
}

Card *Player::playClickedCard(sf::Vector2f mousePosition, Card *firstCard) {
    Card *card = nullptr;
    for (auto c: cards) {
        if (c->getShape().getGlobalBounds().contains(mousePosition)) {
            card = c;
            break;
        }
    }

    // Check if card is playable
    // If there's no first card, the player can play any card
    // If the player can play a card of the same color as the first card, he must play it. If not, he must play any card
    // Colors are E, R, G, S and T

    if (card == nullptr) {
        return nullptr;
    }

    if (firstCard == nullptr) {
        return card;
    }

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
