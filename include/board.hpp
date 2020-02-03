#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<unordered_map>
#include<fstream>

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

#include"textbox.hpp"

/* Orientation */
#define VERTICAL 1
#define HORIZONTAL 0

/* Types of mistakes with adding new word */
#define OUTREACHBOARD 0
#define OVERWRITEWORD 1
#define UNCORRECTWORD 2
#define NONEIGHBOUR 3
#define CORRECTWORD 5


class Board
{
public:
    Board(sf::RenderWindow* window);
    ~Board();

    void draw(sf::RenderWindow* window);

    int addWord(int x, int y, std::vector < std::string > word, bool orientation, sf::RenderWindow* window);

    int countScore(int x, int y, std::vector < std::string > word, bool orientation);
    void debugRANDOMBOARD(sf::RenderWindow* window);

private:
    std::pair < bool, int > checkCorrectness(int x, int y, std::vector < std::string > word, bool orientation);

    void getNewWord(int x, int y, bool orientation, std::vector < std::string >* newWords);

    sf::Texture m_boardTexture;
    sf::Sprite m_boardSprite;

    Textbox* m_lettersImg[15][15];
    std::string m_letters[15][15];
    int m_letterBonus[15][15];
    int m_wordBonus[15][15];

    std::unordered_map <std::string, int> m_dictionary;

    std::map <std::string,int> m_lettersPoints;

    int m_totalWords;

};