#ifndef DOPPELKOPF_CARD_HPP
#define DOPPELKOPF_CARD_HPP

#include <SFML/Graphics.hpp>
#include "WindowSettings.hpp"

class Card : public sf::Drawable {

public:
    Card() = default;

    explicit Card(int num);

    Card(char symbol, char color);

    static int getPointValue(char color);

    static int getPointValue(Card card);

    static int getTrumpValue(char symbol, char color);

    static int getTrumpValue(Card card);

    static char numToSymbol(int num);

    static char numToColor(int num);

    static int colorToNum(char color);

    static int symbolToNum(char symbol);

    void setPosition(sf::Vector2f position) { shape.setPosition(position); };

    void setSize(sf::Vector2f size) { shape.setSize(size); };

    void setRotation(float rotation) { shape.setRotation(rotation); };

    void makeInvisible() { shape.setTexture(&Card::textures[42]); };

    void makeVisible() { shape.setTexture(&Card::textures[colorToNum(color) + symbolToNum(symbol)]); };

    static void
    arrangeAsStack(std::vector<Card *> cards, sf::Vector2f startPosition, sf::Vector2f spacing, float orientation);

    static void loadTextures();

    char color, symbol;

    int trumpValue, pointValue;

    sf::RectangleShape getShape() { return shape; };

private:
    static std::array<sf::Texture, NUMBER_OF_CARDS + 1> textures;
    sf::RectangleShape shape;

    void createShape();

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //DOPPELKOPF_CARD_HPP
