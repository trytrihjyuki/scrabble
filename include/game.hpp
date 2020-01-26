#include<iostream>
#include<vector>
#include<queue>

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

#include"button.hpp"
#include"player.hpp"

class Game{
public:
    Game(sf::RenderWindow* window);
    ~Game();
    void run();

private:
    void draw();
    void processEvents();
    void update(float deltaTime);

    sf::RenderWindow* m_window;

    sf::Texture m_boardTexture;
    sf::Sprite m_boardSprite;

    bool m_exitGame;
};