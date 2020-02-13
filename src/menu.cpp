#include"menu.hpp"

using namespace std;

Menu::Menu(size_t width, size_t height)
{
    m_window = new sf::RenderWindow();
    m_windowWidth = width;
    m_windowHeight = height;
    m_window->create(sf::VideoMode(static_cast<int>(m_windowWidth), static_cast<int>(m_windowHeight)), "scrabble", sf::Style::Close);
    m_window->setFramerateLimit(60);

    m_icon.loadFromFile("static/icon.png");
    m_window->setIcon(m_icon.getSize().x, m_icon.getSize().y, m_icon.getPixelsPtr());

    m_startButton = new Button(sf::Vector2f(m_window->getSize().x / 2 - 150, m_window->getSize().y / 2.3f - 50), sf::Vector2i(250, 75.f),"START",40);
    m_optionButton = new Button(sf::Vector2f(m_window->getSize().x / 2 - 150, m_window->getSize().y / 1.7f - 50), sf::Vector2i(250, 75.f), "OPTIONS",40);
    m_exitButton = new Button(sf::Vector2f(m_window->getSize().x / 2 - 150, m_window->getSize().y / 1.7f + 200 - 50), sf::Vector2i(250, 75.f), "EXIT",40);

    for (int i=0; i<4; i++){ 
        m_players[i] = new Player("PLAYER "+std::to_string(i));
        m_playersButtons[i][0] = new Button(sf::Vector2f(m_window->getSize().x / 4.9f, m_window->getSize().y / 4.f + i*119), sf::Vector2i(400, 75.f), m_players[i]->getName(),50);
        m_playersButtons[i][1] = new Button(sf::Vector2f(m_window->getSize().x / 2.f, m_window->getSize().y / 4.f + i*120), sf::Vector2i(346, 100), m_players[i]->getName(),0);
        m_playersButtons[i][2] = new Button(sf::Vector2f(m_window->getSize().x / 14.f, m_window->getSize().y / 4.f + i*119), sf::Vector2i(100, 75), "ON",50);
        m_playersButtons[i][1]->setImage("static/human_button.png");
    }
    m_activePlayersNum = 4;

    m_exitOptionButton = new Button(sf::Vector2f(m_window->getSize().x / 2 - 150, m_window->getSize().y / 1.7f + 200 - 50), sf::Vector2i(300, 75.f), "GO BACK",40);

    m_titleOptions = new Textbox(sf::Vector2f(sf::Vector2f(m_window->getSize().x / 2.5f, m_window->getSize().y / 7.f)), sf::Vector2i(300, 75.f), "SET PLAYERS NAMES AND TYPES",50);

    m_logoTexture.loadFromFile("static/title_logo.png");
    m_logoTexture.setSmooth(true);
    m_logoSprite.setTexture(m_logoTexture);
    m_logoSprite.setScale(sf::Vector2f((float)m_window->getSize().y / (float)m_logoTexture.getSize().y / 4.f, (float)m_window->getSize().y / (float)m_logoTexture.getSize().y / 4.f));
    m_logoSprite.setPosition(sf::Vector2f(m_window->getSize().x / 2.f - (m_logoTexture.getSize().x * m_logoSprite.getScale().x) / 2.f, m_window->getSize().y / 10.f - 30));

}

Menu::~Menu()
{
}

void Menu::run()
{
    m_exitMenu = false;
    m_optionsActivate = false;

    while (!m_exitMenu)
    {
        draw();
        processEvents();
    }
}

void Menu::draw()
{
    m_window->clear(sf::Color(0, 99, 64));
    if (m_optionsActivate)
    {
        if (m_exitOptionButton->isHover()) m_window->draw(*m_exitOptionButton->getBackgroundPointer());
        m_window->draw(*m_titleOptions->getTextPointer());
        m_window->draw(*m_exitOptionButton->getTextPointer());
        for (int i = 0; i < 4; i++){
            if (m_playersButtons[i][0]->isHover() || m_playersButtons[i][0]->isPressed()) m_window->draw(*m_playersButtons[i][0]->getBackgroundPointer());
            if (m_playersButtons[i][2]->isHover()) m_window->draw(*m_playersButtons[i][2]->getBackgroundPointer());
            m_window->draw(*m_playersButtons[i][0]->getTextPointer());
            m_window->draw(*m_playersButtons[i][1]->getSpritePointer());
            m_window->draw(*m_playersButtons[i][2]->getTextPointer());
        }
        m_window->display();
        return;
    }
    m_window->draw(m_logoSprite);

    if (m_startButton->isHover()) m_window->draw(*m_startButton->getBackgroundPointer());
    if (m_optionButton->isHover()) m_window->draw(*m_optionButton->getBackgroundPointer());
    if (m_exitButton->isHover()) m_window->draw(*m_exitButton->getBackgroundPointer());
    m_window->draw(*m_startButton->getTextPointer());
    m_window->draw(*m_optionButton->getTextPointer());
    m_window->draw(*m_exitButton->getTextPointer());

    m_window->display();
}

void Menu::processEvents()
{
    sf::Event event;

    while (m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_exitMenu = true;
            printf("[+] Exit menu\n");
            break;
        }
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) /*Do not take input after click out of box */
        {
            for (int i = 0; i < 4; i++){
                m_playersButtons[i][0]->updatePress(sf::Mouse::getPosition(*m_window), false);
                m_playersButtons[i][1]->updatePress(sf::Mouse::getPosition(*m_window), false);
            }
        }

        if (m_optionsActivate)
        {
            for (int i = 0; i < 4; i++) m_playersButtons[i][0]->updateHover(sf::Mouse::getPosition(*m_window));
            for (int i = 0; i < 4; i++) m_playersButtons[i][2]->updateHover(sf::Mouse::getPosition(*m_window));
            m_exitOptionButton->updateHover(sf::Mouse::getPosition(*m_window));
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                m_exitOptionButton->updatePress(sf::Mouse::getPosition(*m_window), true);
                if (m_exitOptionButton->isPressed())
                    m_optionsActivate = false;
                /* Activation of input names */
                for (int i = 0; i < 4; i++){
                    m_playersButtons[i][0]->updatePress(sf::Mouse::getPosition(*m_window), true);
                    m_playersButtons[i][1]->updatePress(sf::Mouse::getPosition(*m_window), true);
                    m_playersButtons[i][2]->updatePress(sf::Mouse::getPosition(*m_window), true);
                    if (m_playersButtons[i][0]->isPressed()) printf("[+] Clicked player %d\n",i);
                    if (m_playersButtons[i][1]->isPressed()){
                        printf("[+] Clicked switchbox player no. %d\n",i);
                        if (m_players[i]->getHuman()) m_playersButtons[i][1]->setImage("static/computer_button.png");
                        else m_playersButtons[i][1]->setImage("static/human_button.png");
                        m_players[i]->setHuman(!m_players[i]->getHuman());
                    }
                    if (m_playersButtons[i][2]->isPressed())
                    {
                        printf("[+] Change activity of player no. %d\n",i);
                        if (m_players[i]->getActivate())
                        {
                            if (m_activePlayersNum > 1)
                            {
                                m_players[i]->setActivate(0);
                                m_playersButtons[i][2]->updateText("OFF");
                                m_activePlayersNum--;
                            }
                        }
                        else
                        {
                            m_players[i]->setActivate(1);
                            m_playersButtons[i][2]->updateText("ON");
                            m_activePlayersNum++;
                        }
                    }
                }
            }
            if (event.type == sf::Event::TextEntered) /* Typing players names */
            {
                for (int i = 0; i < 4; i++){
                    if (m_playersButtons[i][0]->isPressed()){
                        if (event.text.unicode == '\b'){
                            if (!m_players[i]->getName().empty()){
                                std::string temp = m_players[i]->getName();
                                if (temp.size() >= 2 && (temp[temp.size()-1] > 'Z' || temp[temp.size()-1] < 'A')) {temp.pop_back(); temp.pop_back();}
                                else  temp.pop_back();
                                m_players[i]->setName(temp);
                                m_players[i]->addNameLength(-1);
                            }
                        }
                        else if (m_players[i]->getNameLength() < 10)
                        {
                            m_players[i]->setName(m_players[i]->getName()+GetCapital(UnicodeToUTF8(event.text.unicode)));
                            if (GetCapital(UnicodeToUTF8(event.text.unicode)) != "") m_players[i]->addNameLength(1);
                        }
                        m_playersButtons[i][0]->updateText(m_players[i]->getName());
                    }
                }
            }
        }
        else {
            m_startButton->updateHover(sf::Mouse::getPosition(*m_window));
            m_optionButton->updateHover(sf::Mouse::getPosition(*m_window));
            m_exitButton->updateHover(sf::Mouse::getPosition(*m_window));
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                m_startButton->updatePress(sf::Mouse::getPosition(*m_window), true);
                m_optionButton->updatePress(sf::Mouse::getPosition(*m_window), true);
                m_exitButton->updatePress(sf::Mouse::getPosition(*m_window), true);
                if (m_startButton->isPressed())
                {
                    printf("[+] Starting game\n");
                    vector <Player*> activePlayers;
                    game = new Game(m_window, m_players);
                    game->run();
                    printf("[+] Ending game\n");
                    delete game;
                    return;
                }
                else
                {
                    if (m_optionButton->isPressed())
                    m_optionsActivate = true;

                  if (m_exitButton->isPressed())
                        m_exitMenu = true;
                }
            }
        }

        if (m_optionsActivate)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                m_optionsActivate = false;
    }
}
