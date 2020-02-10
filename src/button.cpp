#include "button.hpp"

Button::Button(sf::Vector2f position, sf::Vector2i size, std::string text, int characterSize) : Textbox(position,size,text,characterSize)
{
    m_press = false;

    m_hover = false;
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

void Button::updatePress(sf::Vector2i mousePosition, bool buttonPressed)
{
    m_hover = check(mousePosition);
    if (m_hover && buttonPressed) m_press = true;
    else if (check(mousePosition) && !buttonPressed)
    {
        m_press = false;
    }
    else m_press = false;
}

void Button::updateHover(sf::Vector2i mousePosition)
{
    m_hover = check(mousePosition);
}

void Button::setPressed(bool buttonPressed)
{
    m_press = buttonPressed;
}

bool Button::isPressed()
{
    return m_press;
}

bool Button::isHover()
{
    return m_hover;
}