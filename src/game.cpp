#include"game.hpp"

std::string UnicodeToUTF8(unsigned int codepoint)
{
    std::string out;

    if (codepoint <= 0x7f)
        out.append(1, static_cast<char>(codepoint));
    else if (codepoint <= 0x7ff)
    {
        out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else if (codepoint <= 0xffff)
    {
        out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else
    {
        out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    return out;
}

Game::Game(sf::RenderWindow *window,Player* players[4])
{
    m_window = window;

    m_letterBag = {"A","A","A","A","A","A","A","A","A","E","E","E","E","E","E","E","I","I","I","I","I","I","I","I","N","N","N","N","N","O","O","O","O","O","O","R","R","R","R","S","S","S","S","W","W","W","W","Z","Z","Z","Z","Z","C","C","C","D","D","D","K","K","K","L","L","L","M","M","M","P","P","P","T","T","T","Y","Y","Y","Y","B","B","G","G","H","H","J","J","Ł","Ł","U","U","Ą","Ę","F","Ó","Ś","Ż","Ć","Ń","Ź","_","_"};

    for (int i=0; i<4; i++) if (players[i]->getActivate())m_players.push_back(players[i]);
    for (auto player: m_players) player->setRandomLetters(&m_letterBag);
    m_playersNumber = m_players.size();
    srand(time(NULL));
    m_turn = rand()%m_playersNumber;

    m_endGame = 0;
    m_typedWord = "";

    m_board = new Board(m_window);

    /*Score table*/
    m_scoreHeader = new Textbox(sf::Vector2f(m_window->getSize().x / 13.f, m_window->getSize().y / 7.f), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f), "SCORES:",40);
    for (int i=0; i<4; i++) m_scoreTable[i][0] = new Textbox(sf::Vector2f(m_window->getSize().x / 20.f, m_window->getSize().y / 7.f + (i+1)*60), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f), m_players[i]->getName(),30);
    for (int i=0; i<4; i++) m_scoreTable[i][1] = new Textbox(sf::Vector2f(m_window->getSize().x / 6.0f, m_window->getSize().y / 7.f + (i+1)*60), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f), std::to_string(m_players[i]->getScore()),30);

    /* Player stuff, name, tiles etc. */
    m_activePlayerHeader = new Textbox(sf::Vector2f(m_window->getSize().x / 1.2f, m_window->getSize().y / 7.f), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f),"CURRENT PLAYER:",40);
    m_activePlayerName = new Textbox(sf::Vector2f(m_window->getSize().x / 1.2f, m_window->getSize().y / 4.5f), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 9.f),m_players[m_turn]->getName(),40);
    m_playerTilesHeader = new Textbox(sf::Vector2f(m_window->getSize().x / 6.2f, m_window->getSize().y / 1.35f), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f),"YOUR TILES:",30);
    for (int i=0; i<7; i++) m_activePlayerTiles.push_back(new Textbox(sf::Vector2f(m_window->getSize().x / 3.2f + i*60, m_window->getSize().y / 1.3f), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f), "",0));

    /* Enter Word */
    m_enterWordHeader = new Textbox(sf::Vector2f(m_window->getSize().x / 6.2f, m_window->getSize().y / 1.2f), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f),"ENTER WORD:",30);
    m_enterWordButton = new Button(sf::Vector2f(m_window->getSize().x / 3.6f, m_window->getSize().y / 1.18f), sf::Vector2i(549, 72), "WORD",30);
    m_enterWordButton->setImage("static/enter_word.png");

    /*Test letter*/
    //m_board->debugRANDOMBOARD(m_window);
    std::vector < std::string > obiad = {"O", "B", "I", "A", "D"};
    std::vector < std::string > baba = {"B","A","B","A"};
    m_board->addWord(4,4,obiad,VERTICAL, m_window);
    m_board->addWord(4,5,baba,HORIZONTAL, m_window);
    m_board->addWord(6,4,obiad,VERTICAL, m_window);
    /* End of debug */
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
    /* Draw board and BG */
    m_window->clear(sf::Color(0, 99, 64));
    m_board->draw(m_window);

    /* Draw score table */
    m_window->draw(*m_scoreHeader->getTextPointer());
    for (int i=0; i<4; i++) m_window->draw(*m_scoreTable[i][0]->getTextPointer());
    for (int i=0; i<4; i++) m_window->draw(*m_scoreTable[i][1]->getTextPointer());

    /* Draw active player stuff */
    m_window->draw(*m_activePlayerName->getTextPointer());
    m_window->draw(*m_activePlayerHeader->getTextPointer());
    for (auto tile: m_activePlayerTiles) m_window->draw(*tile->getSpritePointer());
    m_window->draw(*m_playerTilesHeader->getTextPointer());

    /* Enter word and action buttons */
    
    /* Enter word */
    m_window->draw(*m_enterWordHeader->getTextPointer());
    m_window->draw(*m_enterWordButton->getSpritePointer());
    m_window->draw(*m_enterWordButton->getTextPointer());


    m_window->display();
}

void Game::processEvents()
{
    sf::Event event;
    sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(*m_window).x, sf::Mouse::getPosition(*m_window).y);

    Player* activePlayer = m_players[m_turn];
    m_activePlayerName->updateText(activePlayer->getName());
    std::vector < std::string > activePlayerLetters = activePlayer->getLetters();
    for(unsigned int i = 0; i < activePlayerLetters.size(); i++) m_activePlayerTiles[i]->setImage(std::string("static/letters/pl/") + activePlayerLetters[i] + ".png");
    // for(int i = activePlayerLetters.size(); i < m_activePlayerTiles.size(); i++) 

    while (m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_exitGame = true;
            printf("[+] Exit game\n");
            break;
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) /*Do not take input after click out of box */
        {
            m_enterWordButton->update(sf::Mouse::getPosition(*m_window), false);
        }

        /* Mouse handle */
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            /* Activation of input names */
            m_enterWordButton->update(sf::Mouse::getPosition(*m_window), true);
            if(m_enterWordButton->isPressed()) printf("[+] Enter word typing ON \n");

        }

        if (event.type == sf::Event::TextEntered) /* Typing players names */
        {
                if (m_enterWordButton->isPressed()){
                    std::cout<<event.text.unicode<<"\n";
                    std::cout<<"ą\nę\nó"<<int('ą')<<"\n"<<int('ę')<<"\n"<<int('ó')<<"\n";
                    if (event.text.unicode == '\b')
                    {
                        if (!m_typedWord.empty()) m_typedWord.pop_back();
                    }
                    else if(m_typedWord.size() <= 15) m_typedWord+= UnicodeToUTF8(event.text.unicode);
                    std::cout<<"debug: "<<UnicodeToUTF8(event.text.unicode)<<"\n";
                    m_enterWordButton->updateText(m_typedWord);
                }
        }
    } 
}
