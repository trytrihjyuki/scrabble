#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "textbox.hpp"

class Button : public Textbox
{
public:
    Button(sf::Vector2f position, sf::Vector2i size, std::string text, int characterSize);
    ~Button();

    bool check(sf::Vector2i mousePosition);

    void update(sf::Vector2i mousePosition, bool buttonPressed);

    bool isPressed();
private:
    bool m_press = false;
};