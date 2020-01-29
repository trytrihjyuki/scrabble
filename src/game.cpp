#include"game.hpp"

using namespace std;


Game::Game(sf::RenderWindow *window,Player* players[4])
{
    m_window = window;
    for(int i=0; i<4; i++) m_players[i] = players[i];

    m_font.loadFromFile("static/Aller_Rg.ttf");

    /*Board*/
    m_boardTexture.loadFromFile("static/plansza.png");
    m_boardTexture.setSmooth(true);
    m_boardSprite.setTexture(m_boardTexture);
    m_boardSprite.setScale(sf::Vector2f((float)m_window->getSize().y / (float)m_boardTexture.getSize().y / 1.4f, (float)m_window->getSize().y / (float)m_boardTexture.getSize().y / 1.4f));
    m_boardSprite.setPosition(sf::Vector2f(m_window->getSize().x / 2.f - (m_boardTexture.getSize().x * m_boardSprite.getScale().x) / 2.f, m_window->getSize().y / 10.f - 70));

    /*Score table*/
    m_scoreHeader = new Textbox(sf::Vector2f(m_window->getSize().x / 15.f, m_window->getSize().y / 7.f), sf::Vector2i(m_window->getSize().x / 20.f, m_window->getSize().y / 7.f), "SCORES:",40);
    for(int i=0; i<4; i++) m_scoreTable[i][0] = new Textbox(sf::Vector2f(m_window->getSize().x / 20.f, m_window->getSize().y / 7.f + (i+1)*60), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f), m_players[i]->getName() + ":\t\t" + std::to_string(m_players[i]->getScore()),30);



}

Game::~Game()
{

}

void Game::run()
{
    m_exitGame = false;
    while (!m_exitGame)
    {
        draw();
        processEvents();
    }
}

void Game::draw()
{
    m_window->clear(sf::Color(0, 99, 64));

    m_window->draw(m_boardSprite);

    /* Draw score table */
    m_window->draw(*m_scoreHeader->getTextPointer());
    for(int i=0; i<4; i++) m_window->draw(*m_scoreTable[i][0]->getTextPointer());

    m_window->display();
}

void Game::processEvents()
{
    sf::Event event;
    sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(*m_window).x, sf::Mouse::getPosition(*m_window).y);

    while (m_window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            m_exitGame = true;
            printf("[+] Exit game\n");
            break;
        }
    }
}
