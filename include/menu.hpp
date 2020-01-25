#pragma once

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
#include"game.hpp"


class Menu{
public:
    Menu(size_t width, size_t height);
    ~Menu();
    void run();

private:
    void draw();
    void processEvents();

    sf::RenderWindow* window;

    size_t windowHeight;
    size_t windowWidth;

    Button* startButton;
    Button* optionButton;
    Button* exitButton;

    sf::Texture logoTexture;
    sf::Sprite logoSprite;

    //Option vars
    Button* playersButtons[4][2]; //[0] - name buton [1] - switch box
    Player* players[4];
    Button* exitOptionButton;
    sf::Text* titleOptions;
    sf::Font font;


    bool exit_menu;
    bool options_activate;
};