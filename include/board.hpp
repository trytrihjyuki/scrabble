#pragma once

#include<iostream>
#include<vector>
#include<string>

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

#include"textbox.hpp"

class Board
{
public:
    Board();
    ~Board();

    int addWord(int x, int y, std::string word);

private:
    Textbox* m_letters[15][15];
    int m_letterBonus[15][15];
    int m_wordBonus[15][15];

};