#include<iostream>
#include<vector>
#include<queue>
#include<string>

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

#include"textbox.hpp"
#include"button.hpp"
#include"player.hpp"
#include"board.hpp"

class Game{
public:
    Game(sf::RenderWindow* window,Player* players[4]);
    ~Game();
    void run();

private:
    void draw();

    void nextTurn();

    void processEvents();

    sf::RenderWindow* m_window;

    std::vector < Player* > m_players;

    Textbox* m_boardLetters[15][15];
    Board* m_board;

    std::vector < std::string > m_letterBag;

    Textbox* m_scoreHeader;
    Textbox* m_scoreTable[4][2];

    Textbox* m_activePlayerName;
    Textbox* m_activePlayerHeader;
    Textbox* m_playerTilesHeader;
    std::vector < Button* > m_activePlayerTiles;

    Textbox* m_enterWordHeader;
    Button* m_enterWordButton;
    std::string m_enterWord;
    int m_enterWordLength;

    Button* m_skipButton;
    Button* m_changeButton;
    Button* m_horizontalButton;
    Button* m_verticalButton;

    bool m_selectingLetters;
    bool m_selectedLetters[7];

    bool m_enterOrientation;

    bool m_exitGame;
    bool m_endGame;

    int m_playersNumber;

    int m_turn;
};

std::string UnicodeToUTF8(unsigned int codepoint);

std::string GetCapital(std::string letter);