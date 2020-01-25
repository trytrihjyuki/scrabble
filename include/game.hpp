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

    void initGameplay(const std::string & lvlPath);

    sf::RenderWindow* window;

    size_t windowHeight;
    size_t windowWidth;

    Button* startButton;
    Button* optionButton;
    Button* exitButton;




    bool exit_menu;
    bool options_activate;
};