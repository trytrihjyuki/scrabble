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

    bool getHuman();

    void setHuman(bool human);

    std::string getName();

    void setName(std::string name);

private:
    std::string m_name;
    char m_letters[7];
    int currentScore;
    bool m_human; // 1 if yes
};