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


#define VERTICAL 1
#define HORIZONTAL 0

class Board
{
public:
    Board(sf::RenderWindow* window);
    ~Board();

    int addWord(int x, int y, std::string word, bool orientation, sf::RenderWindow* window);

    void draw(sf::RenderWindow* window);

private:
    sf::Texture m_boardTexture;
    sf::Sprite m_boardSprite;

    Textbox* m_letters[15][15];
    int m_letterBonus[15][15];
    int m_wordBonus[15][15];

};