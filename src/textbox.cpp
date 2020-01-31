#include "textbox.hpp"

Textbox::Textbox(sf::Vector2f position, sf::Vector2i size, std::string text, int characterSize)
{
    m_position = position;
    m_size = size;

    m_font.loadFromFile("static/Aller_Rg.ttf");
    m_text.setFont(m_font);
    m_text.setString(text);
    m_text.setCharacterSize(characterSize);
    m_text.setFillColor(sf::Color(255, 242, 230));

    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,0);
    m_text.setPosition(sf::Vector2f((position.x + size.x/2.f), position.y + size.y / 4));
}

Textbox::~Textbox()
{
}

sf::Sprite* Textbox::getSpritePointer()
{
    return &m_Sprite;
}

sf::Text* Textbox::getTextPointer()
{
    return &m_text;
}

void Textbox::updateText(std::string text)
{
    m_text.setFont(m_font);
    m_text.setString(text);
    m_text.setFillColor(sf::Color(255, 242, 230));

    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,0);
    m_text.setPosition(sf::Vector2f((m_position.x + m_size.x/2.f), m_position.y + m_size.y / 4));
}

void Textbox::setImage(std::string Texture_src)
{
    m_Texture.loadFromFile(Texture_src);

    m_Sprite.setTexture(m_Texture);

    m_Sprite.setPosition(m_position);
}