#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Button
{
public:
    Button(sf::Vector2f position, sf::Vector2i size, std::string text);
    ~Button();

    bool check(sf::Vector2i mousePosition);

    void update(sf::Vector2i mousePosition, bool buttonPressed);

    bool isPressed();

    void updateText(std::string text);

    void setImage(std::string Texture_src);

    sf::Sprite* getSpritePointer();
    sf::Text* getTextPointer();

    sf::Vector2f m_position;
    sf::Vector2i m_size;

private:

    sf::Texture m_Texture;

    sf::Sprite m_Sprite;

    sf::Font m_font;
    sf::Text m_text;

    bool m_press = false;


};