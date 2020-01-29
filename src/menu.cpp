#include"menu.hpp"

using namespace std;

Menu::Menu(size_t width, size_t height)
{
    m_window = new sf::RenderWindow();
    m_windowWidth = width;
    m_windowHeight = height;
    m_window->create(sf::VideoMode(static_cast<int>(m_windowWidth), static_cast<int>(m_windowHeight)), "scrabble", sf::Style::Close);
    m_window->setFramerateLimit(60);

    m_font.loadFromFile("static/Aller_Rg.ttf");

    m_startButton = new Button(sf::Vector2f(m_window->getSize().x / 2 - 150, m_window->getSize().y / 2.3f - 50), sf::Vector2i(300, 75.f),"START",40);
    m_optionButton = new Button(sf::Vector2f(m_window->getSize().x / 2 - 150, m_window->getSize().y / 1.7f - 50), sf::Vector2i(300, 75.f), "OPTIONS",40);
    m_exitButton = new Button(sf::Vector2f(m_window->getSize().x / 2 - 150, m_window->getSize().y / 1.7f + 200 - 50), sf::Vector2i(300, 75.f), "EXIT",40);

    for(int i=0; i<4; i++){ 
        m_players[i] = new Player("PLAYER "+std::to_string(i));
        m_playersButtons[i][0] = new Button(sf::Vector2f(m_window->getSize().x / 4.f, m_window->getSize().y / 4.f + i*119), sf::Vector2i(300, 75.f), m_players[i]->getName(),50);
        m_playersButtons[i][1] = new Button(sf::Vector2f(m_window->getSize().x / 2.f, m_window->getSize().y / 4.f + i*120), sf::Vector2i(324, 93.f), m_players[i]->getName(),0);
        m_playersButtons[i][1]->setImage("static/human_button2.png");
    }
    m_exitOptionButton = new Button(sf::Vector2f(m_window->getSize().x / 2 - 150, m_window->getSize().y / 1.7f + 200 - 50), sf::Vector2i(300, 75.f), "GO BACK",40);



    m_titleOptions = new sf::Text();
    m_titleOptions->setString("SET PLAYERS NAMES AND TYPES");
    m_titleOptions->setFont(m_font);
    m_titleOptions->setCharacterSize(50);
    m_titleOptions->setColor(sf::Color(255, 242, 230));

    sf::FloatRect textRect = m_titleOptions->getLocalBounds();
    m_titleOptions->setOrigin(textRect.left + textRect.width / 2.f,0);
    m_titleOptions->setPosition(sf::Vector2f((m_window->getSize().x / 2.f), m_window->getSize().y / 7.f));

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
    if(m_optionsActivate)
    {
        m_window->draw(*m_titleOptions);
        m_window->draw(*m_exitOptionButton->getTextPointer());
        for (int i = 0; i < 4; i++){
            m_window->draw(*m_playersButtons[i][0]->getTextPointer());
            m_window->draw(*m_playersButtons[i][1]->getSpritePointer());
        }
        m_window->display();
        return;
    }
    m_window->draw(m_logoSprite);

    m_window->draw(*m_startButton->getTextPointer());
    m_window->draw(*m_optionButton->getTextPointer());
    m_window->draw(*m_exitButton->getTextPointer());

    m_window->display();
}

void Menu::processEvents()
{
    sf::Event event;
    sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(*m_window).x, sf::Mouse::getPosition(*m_window).y);

    while (m_window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            m_exitMenu = true;
            printf("[+] Exit menu\n");
            break;
        }
        if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) //Do not take input after click out of box
        {
            for (int i = 0; i < 4; i++){
                m_playersButtons[i][0]->update(sf::Mouse::getPosition(*m_window), false);
                m_playersButtons[i][1]->update(sf::Mouse::getPosition(*m_window), false);
            }
        }

        if(m_optionsActivate)
        {
            if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                m_exitOptionButton->update(sf::Mouse::getPosition(*m_window), false);
                if(m_exitOptionButton->getTextPointer()->getGlobalBounds().contains(mouse))
                    m_optionsActivate = false;
                //Activation of input names
                for(int i = 0; i < 4; i++){
                    m_playersButtons[i][0]->update(sf::Mouse::getPosition(*m_window), true);
                    m_playersButtons[i][1]->update(sf::Mouse::getPosition(*m_window), true);
                    if(m_playersButtons[i][0]->isPressed()) printf("[+] Clicked player %d\n",i);
                    if(m_playersButtons[i][1]->isPressed()){
                        printf("[+] Clicked switchbox player no. %d\n",i);
                        if(m_players[i]->getHuman()) m_playersButtons[i][1]->setImage("static/computer_button2.png");
                        else m_playersButtons[i][1]->setImage("static/human_button2.png");
                        m_players[i]->setHuman(!m_players[i]->getHuman());
                    }
                }
            }
            if (event.type == sf::Event::TextEntered)
            {
                for(int i = 0; i < 4; i++){
                    if(m_playersButtons[i][0]->isPressed()){
                        if(event.text.unicode == '\b'){
                            if(!m_players[i]->getName().empty()){
                                std::string temp = m_players[i]->getName();
                                temp.pop_back();
                                m_players[i]->setName(temp);
                            }
                        }
                        else
                            m_players[i]->setName(m_players[i]->getName()+(char)event.text.unicode);

                        m_playersButtons[i][0]->updateText(m_players[i]->getName());
                    }
                }
            }
        }
        else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            m_startButton->update(sf::Mouse::getPosition(*m_window), false);
            m_optionButton->update(sf::Mouse::getPosition(*m_window), false);
            m_exitButton->update(sf::Mouse::getPosition(*m_window), false);
            if(m_startButton->getTextPointer()->getGlobalBounds().contains(mouse))
            {
                printf("[+] Starting game\n");
                game = new Game(m_window, m_players);
                game->run();
                printf("[+] Ending game\n");
                delete game;
                return;
            }
            else
            {
                if(m_optionButton->getTextPointer()->getGlobalBounds().contains(mouse))
                    m_optionsActivate = true;

                if(m_exitButton->getTextPointer()->getGlobalBounds().contains(mouse))
                    m_exitMenu = true;
            }
        }
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            if(m_optionsActivate)
            {
                printf("[+] Running options\n");
                break;
            }

            m_startButton->update(sf::Mouse::getPosition(*m_window), true);
            m_optionButton->update(sf::Mouse::getPosition(*m_window), true);
        }


        if(m_optionsActivate)
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                m_optionsActivate = false;
    }
}
