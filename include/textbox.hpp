#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Textbox
{
public:
    Textbox(sf::Vector2f position, sf::Vector2i size, std::string text, int characterSize);
    ~Textbox();

    void updateText(std::string text);

    void setImage(std::string Texture_src);

    sf::Sprite* getSpritePointer();
    sf::Text* getTextPointer();

    sf::Vector2f m_position;
    sf::Vector2i m_size;

protected:
    sf::Texture m_Texture;

    sf::Sprite m_Sprite;

    sf::Font m_font;
    sf::Text m_text;

};