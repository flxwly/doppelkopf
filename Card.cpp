#include <iostream>
#include "Card.hpp"

std::array<sf::Texture, NUMBER_OF_CARDS + 1> Card::textures = {};

int Card::getPointValue(Card card) {
    return getPointValue(card.symbol);
}


int Card::getPointValue(char symbol) {
    switch (symbol) {
        case 'A':
            return 11;
        case '1':
            return 10;
        case 'K':
            return 4;
        case 'O':
            return 3;
        case 'U':
            return 2;
        default:
            std::cerr << "Unexpected value: " << symbol << std::endl;
            return -1;
    };
}

int Card::getTrumpValue(Card card) {
    return getTrumpValue(card.symbol, card.color);
}

int Card::getTrumpValue(char symbol, char color) {
    if (color == 'R' && symbol == '1') {
        return 12;
    }

    if (symbol == 'O') {
        switch (color) {
            case 'E':
                return 11;
            case 'G':
                return 10;
            case 'R':
                return 9;
            case 'S':
                return 8;
            default:
                std::cerr << "Unexpected value: " << symbol << std::endl;
                return -100;
        };
    }
    if (symbol == 'U') {
        switch (color) {
            case 'E':
                return 7;
            case 'G':
                return 6;
            case 'R':
                return 5;
            case 'S':
                return 4;
            default:
                std::cerr << "Unexpected value: " << symbol << std::endl;
                return -100;
        };
    }
    if (color == 'S') {
        switch (symbol) {
            case 'A':
                return 3;
            case 'K':
                return 2;
            case '1':
                return 1;
            default:
                std::cerr << "Unexpected value: " << symbol << std::endl;
                return -100;
        };
    }

    switch (symbol) {
        case 'A':
            return -1;
        case 'K':
            return -2;
        case '1':
            return -3;
        default:
            std::cerr << "Unexpected value: " << symbol << std::endl;
            return -100;
    };
}

void Card::createShape() {
    shape.setSize(sf::Vector2f(WINDOW_WIDTH / 30, WINDOW_HEIGHT / 20));
    makeVisible();
}

Card::Card(int num) : color(numToColor(num)), symbol(numToSymbol(num)),
                      trumpValue(getTrumpValue(numToSymbol(num), numToColor(num))),
                      pointValue(getPointValue(numToSymbol(num))) {

    createShape();
}

Card::Card(char color, char symbol) : color(color), symbol(symbol),
                                      trumpValue(getTrumpValue(symbol, color)),
                                      pointValue(getPointValue(symbol)) {

    createShape();
}

char Card::numToSymbol(int num) {
    if (num % 10 == 0 || num % 10 == 1) {
        return 'A';
    }
    if (num % 10 == 2 || num % 10 == 3) {
        return '1';
    }
    if (num % 10 == 4 || num % 10 == 5) {
        return 'K';
    }
    if (num % 10 == 6 || num % 10 == 7) {
        return 'O';
    }
    if (num % 10 == 8 || num % 10 == 9) {
        return 'U';
    }
    std::cerr << "Unexpected value: " << num << std::endl;
    return 'X';
}

char Card::numToColor(int num) {
    if (num >= 0 && num < 10) {
        return 'E';
    } else if (num >= 10 && num < 20) {
        return 'G';
    } else if (num >= 20 && num < 30) {
        return 'R';
    } else if (num >= 30 && num < 40) {
        return 'S';
    } else {
        std::cerr << "Unexpected value: " << num << std::endl;
        return 'X';
    }
}

int Card::colorToNum(char color) {
    switch (color) {
        case 'E':
            return 0;
        case 'G':
            return 10;
        case 'R':
            return 20;
        case 'S':
            return 30;
        default:
            std::cerr << "Unexpected value: " << color << std::endl;
            return -1;
    }
}

int Card::symbolToNum(char symbol) {
    switch (symbol) {
        case 'A':
            return 0;
        case '1':
            return 2;
        case 'K':
            return 4;
        case 'O':
            return 6;
        case 'U':
            return 8;
        default:
            std::cerr << "Unexpected value: " << symbol << std::endl;
            return -1;
    }
}

void Card::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(shape);
}

void Card::arrangeAsStack(std::vector<Card *> cards, sf::Vector2f startPosition, sf::Vector2f spacing, float orientation) {
    for (int i = 0; i < cards.size(); i++) {
        cards[i]->setPosition(startPosition + static_cast<float>(i) * spacing);
        cards[i]->setRotation(orientation);
    }
}



void Card::loadTextures() {
    for (int i = 0; i < 40; i += 2) {
        sf::Texture texture;
        if (!texture.loadFromFile("../recources/cards/" + std::to_string(i) + ".jpg")) {
            std::cerr << "Error loading card texture" << std::endl;
            exit(999);
        }
        Card::textures[i] = texture;
        Card::textures[i + 1] = texture;
    }
}
