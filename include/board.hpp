#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<map>

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

    void draw(sf::RenderWindow* window);

    int addWord(int x, int y, std::vector < std::string > word, bool orientation, sf::RenderWindow* window);

    int countScore(/*int x, int y, std::vector < std::string > word, bool orientation*/);
    void debugRANDOMBOARD(sf::RenderWindow* window);

private:
    bool checkCorrectness(int x, int y, std::vector < std::string > word, bool orientation);

    void getNewWord(int x, int y, bool orientation, std::vector < std::string >* newWords);

    sf::Texture m_boardTexture;
    sf::Sprite m_boardSprite;

    Textbox* m_lettersImg[15][15];
    std::string m_letters[15][15];
    int m_letterBonus[15][15];
    int m_wordBonus[15][15];

    std::map <std::string,int> m_lettersPoints;

};