#include "player.hpp"

Player::Player(std::string name)
{
    m_name = name;
    m_nameLength = name.size(); 

    m_activate = 1;
    m_human = 1;
    m_score = 0;
}

Player::~Player()
{

}

bool Player::getActivate()
{
    return m_activate;
}

void Player::setActivate(bool activate)
{
    m_activate = activate;
}

bool Player::getHuman()
{
    return m_human;
}

void Player::setHuman(bool human)
{
    m_human = human;
}

std::string Player::getName()
{
    return m_name;
}

void Player::setName(std::string name)
{
    m_name = name;
}

void Player::addNameLength(int diff)
{
    m_nameLength+=diff;
}

int Player::getNameLength()
{
    return m_nameLength;
}

int Player::getScore()
{
    return m_score;
}

void Player::setScore(int score)
{
    m_score = score;
}

std::vector < std::string > Player::getLetters()
{
    return m_letters;
}

void Player::setRandomLetters(std::vector < std::string >* letterBag)
{
    srand(time(NULL));
    std::random_shuffle (letterBag->begin(), letterBag->end());
    m_letters.clear();
    for(int i = 0; i < std::min(7, (int)letterBag->size()); i++)
    {
        m_letters.push_back((*letterBag)[i]);
        letterBag->erase(letterBag->begin());
    }
}