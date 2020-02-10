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
    m_text.setOutlineThickness(1);

    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.f,0);
    m_text.setPosition(sf::Vector2f(position.x + size.x/2.f, position.y + size.y/5.f));

    m_background.setPosition(m_position);
    m_background.setSize(sf::Vector2f(size.x,size.y));
    m_background.setFillColor(sf::Color(0, 66, 42));

}

Textbox::~Textbox()
{
}

sf::Sprite* Textbox::getSpritePointer()
{
    return &m_sprite;
}

sf::Text* Textbox::getTextPointer()
{
    return &m_text;
}

sf::RectangleShape* Textbox::getBackgroundPointer()
{
    return &m_background;
}

void Textbox::updateText(std::string text)
{
    m_text.setFont(m_font);
    m_text.setString(sf::String::fromUtf8(text.begin(), text.end()));
    m_text.setFillColor(sf::Color(255, 242, 230));

    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,0);
    m_text.setPosition(sf::Vector2f((m_position.x + m_size.x/2.f), m_position.y + m_size.y / 4));
}

void Textbox::setImage(std::string Texture_src)
{
    m_texture.loadFromFile(Texture_src);

    m_sprite.setTexture(m_texture);

    m_sprite.setPosition(m_position);
}