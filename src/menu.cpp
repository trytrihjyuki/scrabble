#include"menu.hpp"

using namespace std;

Menu::Menu(size_t width, size_t height)
{
    window = new sf::RenderWindow();
    windowWidth = width;
    windowHeight = height;
    window->create(sf::VideoMode(static_cast<int>(windowWidth), static_cast<int>(windowHeight)), "scrabble", sf::Style::Close);
    window->setFramerateLimit(60);

    font.loadFromFile("static/Aller_Rg.ttf");

    startButton = new Button(sf::Vector2f(window->getSize().x / 2 - 150, window->getSize().y / 2.3f - 50), sf::Vector2i(300, 75.f),"START");
    optionButton = new Button(sf::Vector2f(window->getSize().x / 2 - 150, window->getSize().y / 1.7f - 50), sf::Vector2i(300, 75.f), "OPTIONS");
    exitButton = new Button(sf::Vector2f(window->getSize().x / 2 - 150, window->getSize().y / 1.7f + 200 - 50), sf::Vector2i(300, 75.f), "EXIT");

    for(int i=0; i<4; i++){ 
        players[i] = new Player("PLAYER "+std::to_string(i));
        playersButtons[i][0] = new Button(sf::Vector2f(window->getSize().x / 3 - 150, window->getSize().y / 4.f + i*119), sf::Vector2i(300, 75.f), players[i]->getName());
        playersButtons[i][1] = new Button(sf::Vector2f(window->getSize().x / 3 + 150, window->getSize().y / 4.f + i*120), sf::Vector2i(243, 70.f), players[i]->getName());
        playersButtons[i][1]->setImage("static/human_button2.png");
    }
    exitOptionButton = new Button(sf::Vector2f(window->getSize().x / 2 - 150, window->getSize().y / 1.7f + 200 - 50), sf::Vector2i(300, 75.f), "GO BACK");



    titleOptions = new sf::Text();
    titleOptions->setString("SET PLAYERS NAMES AND TYPES");
    titleOptions->setFont(font);
    titleOptions->setColor(sf::Color(255, 242, 230));

    sf::FloatRect textRect = titleOptions->getLocalBounds();
    titleOptions->setOrigin(textRect.left + textRect.width / 2.0f,0);
    titleOptions->setPosition(sf::Vector2f((window->getSize().x / 2), window->getSize().y / 7.f));

    logoTexture.loadFromFile("static/title_logo.png");
    logoTexture.setSmooth(true);
    logoSprite.setTexture(logoTexture);
    logoSprite.setScale(sf::Vector2f((float)window->getSize().y / (float)logoTexture.getSize().y / 4.f, (float)window->getSize().y / (float)logoTexture.getSize().y / 4.f));
    logoSprite.setPosition(sf::Vector2f(window->getSize().x / 2.f - (logoTexture.getSize().x * logoSprite.getScale().x) / 2.f, window->getSize().y / 10.f - 30));

}

Menu::~Menu()
{

}

void Menu::run()
{
    exit_menu = false;
    options_activate = false;

    while (!exit_menu)
    {
        draw();
        processEvents();
    }
}

void Menu::draw()
{
    window->clear(sf::Color(0, 99, 64));
    if(options_activate)
    {
        window->draw(*titleOptions);
        window->draw(*exitOptionButton->getTextPointer());
        for (int i = 0; i < 4; i++){
            window->draw(*playersButtons[i][0]->getTextPointer());
            window->draw(*playersButtons[i][1]->getSpritePointer());
        }
        window->display();
        return;
    }
    window->draw(logoSprite);

    window->draw(*startButton->getTextPointer());
    window->draw(*optionButton->getTextPointer());
    window->draw(*exitButton->getTextPointer());

    window->display();
}

void Menu::processEvents()
{
    sf::Event event;
    sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);

    while (window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            exit_menu = true;
            printf("[+] Exit menu\n");
            break;
        }
        if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) //Do not take input after click out of box
        {
            for (int i = 0; i < 4; i++){
                playersButtons[i][0]->update(sf::Mouse::getPosition(*window), false);
                playersButtons[i][1]->update(sf::Mouse::getPosition(*window), false);
            }
        }

        if(options_activate)
        {
            if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                exitOptionButton->update(sf::Mouse::getPosition(*window), false);
                if(exitOptionButton->getTextPointer()->getGlobalBounds().contains(mouse))
                    options_activate = false;
                //Activation of input names
                for(int i = 0; i < 4; i++){
                    playersButtons[i][0]->update(sf::Mouse::getPosition(*window), true);
                    playersButtons[i][1]->update(sf::Mouse::getPosition(*window), true);
                    if(playersButtons[i][0]->isPressed()) printf("Clicked player %d\n",i);
                    if(playersButtons[i][1]->isPressed()){
                        printf("Clicked switchbox player no. %d\n",i);
                        if(players[i]->getHuman()) playersButtons[i][1]->setImage("static/computer_button2.png");
                        else playersButtons[i][1]->setImage("static/human_button2.png");
                        players[i]->setHuman(!players[i]->getHuman());
                    }
                }
            }
            if (event.type == sf::Event::TextEntered)
            {
                for(int i = 0; i < 4; i++){
                    if(playersButtons[i][0]->isPressed()){
                        if(event.text.unicode == '\b'){
                            if(!players[i]->getName().empty()){
                                std::string temp = players[i]->getName();
                                temp.pop_back();
                                players[i]->setName(temp);
                            }
                        }
                        else
                            players[i]->setName(players[i]->getName()+(char)event.text.unicode);

                        playersButtons[i][0]->updateText(players[i]->getName());
                    }
                }
            }
        }
        else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            startButton->update(sf::Mouse::getPosition(*window), false);
            optionButton->update(sf::Mouse::getPosition(*window), false);
            exitButton->update(sf::Mouse::getPosition(*window), false);
            if(startButton->getTextPointer()->getGlobalBounds().contains(mouse))
            {
                printf("[+] Starting game\n");
                return;
            }
            else
            {
                if(optionButton->getTextPointer()->getGlobalBounds().contains(mouse))
                    options_activate = true;

                if(exitButton->getTextPointer()->getGlobalBounds().contains(mouse))
                    exit_menu = true;
            }
        }
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            if(options_activate)
            {
                printf("[+] Running options\n");
                break;
            }

            startButton->update(sf::Mouse::getPosition(*window), true);
            optionButton->update(sf::Mouse::getPosition(*window), true);
        }


        if(options_activate)
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                options_activate = false;
    }
}
