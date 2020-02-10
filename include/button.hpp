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

    void updatePress(sf::Vector2i mousePosition, bool buttonPressed);

    void updateHover(sf::Vector2i mousePosition);

    void setPressed(bool buttonPressed);

    bool isPressed();

    bool isHover();

private:
    bool m_press;

    bool m_hover;
};