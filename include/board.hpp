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
#include"button.hpp"

/* Do not output debug when bruting */
#define NOBRUT if(!m_makingBestMove)

/* Orientation */
#define VERTICAL 1
#define HORIZONTAL 0

/* Types of mistakes with adding new word */
#define OUTREACHBOARD 0
#define OVERWRITEWORD 1
#define UNCORRECTWORD 2
#define NONEIGHBOUR 3
#define MISSINGLETTERS 4
#define SAMEWORD 5
#define NOMIDDLEFIRSTWORD 6
#define CORRECTWORD 7
#define CORRECTWORD50BONUS 8

struct Move
{
    int x;
    int y;
    bool orientation;
    std::vector <std::string> word;
    int score;
};

struct WordOnBoard
{
    int x;
    int y;
    bool orientation;
    std::string word;
};

class Board
{
public:
    Board(sf::RenderWindow* window);
    ~Board();

    void draw(sf::RenderWindow* window);

    bool checkTilePress(int x, int y, sf::Vector2i mousePosition);

    std::pair <int, std::vector <int> > checkWord(Move move, std::vector < std::string > playersLetters);

    int addWord(Move move, std::vector < std::string > playersLetters);

    int countScore(Move move, std::vector <int> blanks);

    std::vector <std::string> unusedLetters(Move move, std::vector < std::string > playersLetters);

    Move getBestMove(std::vector < std::string > playersLetters);

    int summLetters(std::vector <std::string> letters);
private:
    std::pair < bool, std::pair < int, std::vector <int> > > checkCorrectness(Move move, std::vector < std::string > playersLetters);

    void getNewWord(int x, int y, bool orientation, std::vector < WordOnBoard >* newWords, bool newWordFlag);

    sf::Texture m_boardTexture;
    sf::Sprite m_boardSprite;

    Button* m_lettersTiles[15][15];
    std::string m_letters[15][15];
    int m_letterBonus[15][15];
    int m_wordBonus[15][15];

    std::unordered_map <std::string, int> m_dictionary;
    std::vector <std::string> m_alphabet;

    std::map <std::string,int> m_lettersPoints;

    int m_totalWords;
    std::vector <WordOnBoard> m_allWords;


    bool m_makingBestMove;
    bool m_addingWord;
};

std::vector <std::string> convertString(std::string word);