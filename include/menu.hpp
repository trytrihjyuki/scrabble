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

    Game* game;

    sf::RenderWindow* m_window;

    size_t m_windowHeight;
    size_t m_windowWidth;

    Button* m_startButton;
    Button* m_optionButton;
    Button* m_exitButton;

    sf::Texture m_logoTexture;
    sf::Sprite m_logoSprite;

    //Option vars
    Button* m_playersButtons[4][2]; //[0] - name buton [1] - switch box
    Player* m_players[4];
    Button* m_exitOptionButton;
    Textbox* m_titleOptions;


    bool m_exitMenu;
    bool m_optionsActivate;
};