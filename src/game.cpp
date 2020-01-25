#include"game.hpp"

using namespace std;


Game::Game(sf::RenderWindow *window_game)
{
    window = window_game;
}

Game::~Game()
{

}

void Game::run()
{
    exit_menu = false;
    options_activate = false;

    while (!exit_menu)
    {
        draw();
        processEvents();
        if(options_activate)
            options();
    }
}

void Game::draw()
{


}

void Game::processEvents()
{

}
