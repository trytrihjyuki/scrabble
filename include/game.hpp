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
    void processEvents();

    sf::RenderWindow* m_window;

    std::vector <Player*> m_players;

    Textbox* m_boardLetters[15][15];
    Board* m_board;

    Textbox* m_scoreHeader;
    Textbox* m_scoreTable[4][2];

    bool m_exitGame;

    int m_playersNumber;
};