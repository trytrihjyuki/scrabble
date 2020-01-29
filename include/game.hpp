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

class Game{
public:
    Game(sf::RenderWindow* window,Player* players[4]);
    ~Game();
    void run();

private:
    void draw();
    void processEvents();
    void update(float deltaTime);

    sf::RenderWindow* m_window;

    sf::Texture m_boardTexture;
    sf::Sprite m_boardSprite;
    Player* m_players[4];
    Textbox* m_scoreHeader;
    Textbox* m_scoreTable[4][2];
    sf::Font m_font;

    bool m_exitGame;
};