#pragma once

#include<iostream>
#include<vector>
#include<string>

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
private:
    std::string m_name;
    char m_letters[7];
    int m_score;
    bool m_human;
    bool m_activate;
};