#include "button.hpp"

Button::Button(sf::Vector2f position, sf::Vector2i size, std::string text, int characterSize)
{
    m_position = position;
    m_size = size;

    m_font.loadFromFile("static/Aller_Rg.ttf");
    m_text.setFont(m_font);
    m_text.setString(text);
    m_text.setCharacterSize(characterSize);
    m_text.setColor(sf::Color(255, 242, 230));

    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,0);
    m_text.setPosition(sf::Vector2f((position.x + size.x/2.f), position.y + size.y / 4));
}

Button::~Button()
{
}

bool Button::check(sf::Vector2i mousePosition)
{
    if (mousePosition.x > m_position.x && mousePosition.x < m_position.x + m_size.x)
    {
        if (mousePosition.y > m_position.y && mousePosition.y < m_position.y + m_size.y)
        {
            return true;
        }
        else return false;
    }
    else return false;
}


void Button::update(sf::Vector2i mousePosition, bool buttonPressed)
{
    if (check(mousePosition) && buttonPressed) m_press = true;
    else if (check(mousePosition) && !buttonPressed)
    {
        m_press = false;
    }
    else m_press = false;
}

sf::Sprite* Button::getSpritePointer()
{
    return &m_Sprite;
}

sf::Text* Button::getTextPointer()
{
    return &m_text;
}

bool Button::isPressed()
{
    return m_press;
}

void Button::updateText(std::string text)
{
    m_text.setFont(m_font);
    m_text.setString(text);
    m_text.setColor(sf::Color(255, 242, 230));

    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,0);
    m_text.setPosition(sf::Vector2f((m_position.x + m_size.x/2.f), m_position.y + m_size.y / 4));
}

void Button::setImage(std::string Texture_src)
{
    m_Texture.loadFromFile(Texture_src);

    m_Sprite.setTexture(m_Texture);

    m_Sprite.setScale(sf::Vector2f((float)m_size.x / (float)m_Texture.getSize().x, (float)m_size.y / (float)m_Texture.getSize().y));

    m_Sprite.setPosition(m_position);
}