#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>


class Player
{
public:

    Player(std::string);
    ~Player();

    bool getActivate();

    void setActivate(bool activate);

    bool getHuman();

    void setHuman(bool human);

    std::string getName();

    void setName(std::string name);

    int getScore();

    void setScore(int Score);

    std::vector < std::string > getLetters();

    void setRandomLetters(std::vector < std::string >* letterBag);
private:
    std::string m_name;
    bool m_activate;
    bool m_human;

    std::vector < std::string > m_letters;
    int m_score;

};