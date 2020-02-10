#include"game.hpp"

/* Functions to handle UTF8 */
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

std::string GetCapital(std::string letter)
{
    if (letter.size() == 1)
    {
        if (letter[0] >= 'a' && letter[0] <= 'z') {letter[0]-=32; return letter;}
        if (letter[0] >= 'A' && letter[0] <= 'Z') return letter;
    }
    else
    {
        if (letter == "ą" || letter == "Ą") return "Ą";
        if (letter == "ę" || letter == "Ę") return "Ę";
        if (letter == "ł" || letter == "Ł") return "Ł";
        if (letter == "ó" || letter == "Ó") return "Ó";
        if (letter == "ć" || letter == "Ć") return "Ć";
        if (letter == "ń" || letter == "Ń") return "Ń";
        if (letter == "ś" || letter == "Ś") return "Ś";
        if (letter == "ż" || letter == "Ż") return "Ż";
        if (letter == "ź" || letter == "Ź") return "Ź";
    }

    return "";
}

Game::Game(sf::RenderWindow *window,Player* players[4])
{
    m_window = window;

    m_letterBag = {"A","A","A","A","A","A","A","A","A","E","E","E","E","E","E","E","I","I","I","I","I","I","I","I","N","N","N","N","N","O","O","O","O","O","O","R","R","R","R","S","S","S","S","W","W","W","W","Z","Z","Z","Z","Z","C","C","C","D","D","D","K","K","K","L","L","L","M","M","M","P","P","P","T","T","T","Y","Y","Y","Y","B","B","G","G","H","H","J","J","Ł","Ł","U","U","Ą","Ę","F","Ó","Ś","Ż","Ć","Ń","Ź","_","_"};

    for (int i=0; i<4; i++) if (players[i]->getActivate())m_players.push_back(players[i]);
    for (auto player: m_players) player->setRandomLetters(&m_letterBag,7);
    m_playersNumber = m_players.size();
    srand(time(NULL));
    m_turn = rand()%m_playersNumber;

    m_endGame = 0;
    m_enterWord = "";
    m_enterWordLength = 0;

    m_board = new Board(m_window);

    /*Score table*/
    m_scoreHeader = new Textbox(sf::Vector2f(m_window->getSize().x / 13.f, m_window->getSize().y / 7.f), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f), "SCORES:",40);
    for (int i=0; i<4; i++) m_scoreTable[i][0] = new Textbox(sf::Vector2f(m_window->getSize().x / 20.f, m_window->getSize().y / 7.f + (i+1)*60), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f), m_players[i]->getName(),30);
    for (int i=0; i<4; i++) m_scoreTable[i][1] = new Textbox(sf::Vector2f(m_window->getSize().x / 6.0f, m_window->getSize().y / 7.f + (i+1)*60), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f), std::to_string(m_players[i]->getScore()),30);

    /* Player stuff, name, tiles etc. */
    m_activePlayerHeader = new Textbox(sf::Vector2f(m_window->getSize().x / 1.2f, m_window->getSize().y / 7.f), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f),"CURRENT PLAYER:",40);
    m_activePlayerName = new Textbox(sf::Vector2f(m_window->getSize().x / 1.2f, m_window->getSize().y / 4.5f), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 9.f),m_players[m_turn]->getName(),40);
    m_playerTilesHeader = new Textbox(sf::Vector2f(m_window->getSize().x / 6.2f, m_window->getSize().y / 1.35f), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f),"YOUR TILES:",30);
    for (int i=0; i<7; i++) m_activePlayerTiles.push_back(new Button(sf::Vector2f(m_window->getSize().x / 3.2f + i*60, m_window->getSize().y / 1.3f), sf::Vector2i(m_window->getSize().x / 29.75f, m_window->getSize().y / 20.145f), "",0));

    /* Enter word and action buttons */
    m_enterWordHeader = new Textbox(sf::Vector2f(m_window->getSize().x / 6.2f, m_window->getSize().y / 1.2f), sf::Vector2i(m_window->getSize().x / 12.f, m_window->getSize().y / 7.f),"ENTER WORD:",30);
    m_enterWordButton = new Button(sf::Vector2f(m_window->getSize().x / 3.6f, m_window->getSize().y / 1.18f), sf::Vector2i(549, 72), "WORD",30);
    m_enterWordButton->setImage("static/enter_word.png");

    m_skipButton = new Button(sf::Vector2f(m_window->getSize().x / 1.2f, m_window->getSize().y / 1.205f), sf::Vector2i(100, 100),"",0);
    m_skipButton->setImage("static/skip_button.png");

    m_changeButton = new Button(sf::Vector2f(m_window->getSize().x / 1.11f, m_window->getSize().y / 1.205f), sf::Vector2i(100, 100),"",0);
    m_changeButton->setImage("static/change_button.png");

    m_horizontalButton = new Button(sf::Vector2f(m_window->getSize().x / 1.45f, m_window->getSize().y / 1.235f), sf::Vector2i(180, 50),"HORIZONTAL",25);
    m_verticalButton = new Button(sf::Vector2f(m_window->getSize().x / 1.45f, m_window->getSize().y / 1.125f), sf::Vector2i(180, 50),"VERTICAL",25);

    m_verticalButton->setPressed(true);
    m_enterOrientation = VERTICAL;

    m_selectingLetters = false;

    /*Test letter*/
    m_board->debugRANDOMBOARD();

    /* Initialize first player */
    nextTurn();
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
    for (int i=0; i<m_playersNumber; i++) m_window->draw(*m_scoreTable[i][0]->getTextPointer());
    for (int i=0; i<m_playersNumber; i++) m_window->draw(*m_scoreTable[i][1]->getTextPointer());

    /* Draw active player stuff */
    m_window->draw(*m_activePlayerName->getTextPointer());
    m_window->draw(*m_activePlayerHeader->getTextPointer());
    for (auto tile: m_activePlayerTiles) m_window->draw(*tile->getSpritePointer());
    m_window->draw(*m_playerTilesHeader->getTextPointer());

    /* Enter word and action buttons */
    m_window->draw(*m_skipButton->getSpritePointer());
    if (m_verticalButton->isPressed() || m_verticalButton->isHover()) m_window->draw(*m_verticalButton->getBackgroundPointer());
    if (m_horizontalButton->isPressed() || m_horizontalButton->isHover()) m_window->draw(*m_horizontalButton->getBackgroundPointer());
    m_window->draw(*m_horizontalButton->getTextPointer());
    m_window->draw(*m_verticalButton->getTextPointer());

    m_window->draw(*m_changeButton->getSpritePointer());
    // for (int i = 0; i < 7; i++) if (m_selectedLetters[i]) m_window->draw(*m_activePlayerTiles[i]->getBackgroundPointer());

    m_window->draw(*m_enterWordHeader->getTextPointer());
    m_window->draw(*m_enterWordButton->getSpritePointer());
    m_window->draw(*m_enterWordButton->getTextPointer());


    m_window->display();
}

void Game::nextTurn()
{
    printf("\n[+] NEXT TURN!\n");
    m_turn = (m_turn+1) % m_playersNumber;

    Player* activePlayer = m_players[m_turn];
    m_activePlayerName->updateText(activePlayer->getName());
    std::vector < std::string > activePlayerLetters = activePlayer->getLetters();
    for (unsigned int i = 0; i < activePlayerLetters.size(); i++) m_activePlayerTiles[i]->setImage(std::string("static/letters/pl/") + activePlayerLetters[i] + ".png");
    for (unsigned int i = activePlayerLetters.size(); i < 7; i++) m_activePlayerTiles.pop_back();
    for (unsigned int i = 0; i < m_activePlayerTiles.size(); i++) m_selectedLetters[i] = 0;
    m_selectingLetters = 0;

    m_enterWordButton->updateText("WORD");
    m_enterWord = "";
    m_enterWordLength = 0;
}

void Game::processEvents()
{
    sf::Event event;
    sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(*m_window).x, sf::Mouse::getPosition(*m_window).y);

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
            m_enterWordButton->updatePress(sf::Mouse::getPosition(*m_window), false);
        }

        /* Update Hover */
        m_verticalButton->updateHover(sf::Mouse::getPosition(*m_window));
        m_horizontalButton->updateHover(sf::Mouse::getPosition(*m_window));
        m_changeButton->updateHover(sf::Mouse::getPosition(*m_window));
        m_skipButton->updateHover(sf::Mouse::getPosition(*m_window));
        for (unsigned int i = 0; i < m_activePlayerTiles.size(); i++) m_activePlayerTiles[i]->updateHover(sf::Mouse::getPosition(*m_window));

        /* Highlighting buttons */
        if (m_skipButton->isHover()) m_skipButton->setSpriteColor(sf::Color(125,125,126));
        else m_skipButton->setSpriteColor(sf::Color(255,255,255));

        if (m_changeButton->isHover() || m_selectingLetters) m_changeButton->setSpriteColor(sf::Color(125,125,126));
        else m_changeButton->setSpriteColor(sf::Color(255,255,255));

        for (unsigned int i = 0; i < m_activePlayerTiles.size(); i++)
        {
            if (!m_selectingLetters) m_activePlayerTiles[i]->setSpriteColor(sf::Color(255,255,255));
            if (m_selectingLetters && m_activePlayerTiles[i]->isHover()) m_activePlayerTiles[i]->setSpriteColor(sf::Color(0,180,90));
            if (m_selectingLetters && !m_activePlayerTiles[i]->isHover() && !m_selectedLetters[i]) m_activePlayerTiles[i]->setSpriteColor(sf::Color(255,255,255));
        }
        /* Mouse handle */
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            /* Activation of input names */
            m_enterWordButton->updatePress(sf::Mouse::getPosition(*m_window), true);
            if (m_enterWordButton->isPressed()) printf("[+] Enter word typing ON \n");

            /* Activation skip and change buttons */
            m_changeButton->updatePress(sf::Mouse::getPosition(*m_window), true);
            m_skipButton->updatePress(sf::Mouse::getPosition(*m_window), true);

            /* Skipping turn */
            if (m_skipButton->isPressed())
            {
                printf("[+] Skipping turn.\n");
                nextTurn();
            }

            /* Changing letters */
            if (m_changeButton->isPressed())
            {
                m_selectingLetters = !m_selectingLetters;
                if (m_selectingLetters) printf("[+] Start changing letters.\n");
                else
                {
                    std::vector <std::string> lettersToChange; lettersToChange.resize(0);
                    std::vector <std::string> lettersToSave; lettersToSave.resize(0);
                    for (unsigned int  i = 0; i < m_activePlayerTiles.size(); i++)
                    {
                        if (m_selectedLetters[i]) lettersToChange.push_back(m_players[m_turn]->getLetters()[i]);
                        else lettersToSave.push_back(m_players[m_turn]->getLetters()[i]);
                    }
                    if (lettersToChange.size() > m_letterBag.size()) 
                    {
                        printf("[-] Not enough letters in letterbag. Can't change it.\n");
                        m_selectingLetters = 0;
                        break;
                    }
                    else
                    {
                        printf("[+] Changing this letters:\n");
                        for (auto letter: lettersToChange) printf("%s ",letter.c_str());
                        m_players[m_turn]->setRandomLetters(&m_letterBag,(int)lettersToChange.size());
                        for (auto letter: lettersToChange) m_letterBag.push_back(letter);
                        for (auto letter: lettersToSave) m_players[m_turn]->setLetter(letter);
                        nextTurn();
                    }
                }
            }
            if (m_selectingLetters)
            {
                for (auto tiles: m_activePlayerTiles) tiles->updatePress(sf::Mouse::getPosition(*m_window), true);
                for (unsigned int i = 0; i < m_activePlayerTiles.size(); i++) if (m_activePlayerTiles[i]->isPressed()) std::cout<<"Pressed number "<<i<<"\n";
                for (unsigned int i = 0; i < m_activePlayerTiles.size(); i++) if (m_activePlayerTiles[i]->isPressed()) m_selectedLetters[i] = !m_selectedLetters[i];
                for (unsigned int i = 0; i < m_activePlayerTiles.size(); i++)
                {
                    if (m_selectedLetters[i]) m_activePlayerTiles[i]->setSpriteColor(sf::Color(0, 180, 90));
                    else m_activePlayerTiles[i]->setSpriteColor(sf::Color(255,255,255));
                }
            }

            /* Activation orientation buttons */
            m_verticalButton->updatePress(sf::Mouse::getPosition(*m_window), true);
            m_horizontalButton->updatePress(sf::Mouse::getPosition(*m_window), true);
            if (!m_verticalButton->isPressed() && !m_horizontalButton->isPressed())
            {
                if (m_enterOrientation == VERTICAL) m_verticalButton->setPressed(true);
                if (m_enterOrientation == HORIZONTAL) m_horizontalButton->setPressed(true);
            }
            else
            {
                if (m_verticalButton->isPressed()) m_enterOrientation = VERTICAL;
                else m_enterOrientation = HORIZONTAL;
            }
            std::vector < std::pair <int, int> > clickedTiles; clickedTiles.resize(0);
            for(int x=0; x<15; x++)
            {
                for(int y=0; y<15; y++)
                {
                    if (m_board->checkTilePress(x,y,sf::Mouse::getPosition(*m_window))) clickedTiles.push_back({x,y});
                }
            }

            /* Only enter word if specific cliced */
            if (clickedTiles.size() == 1)
            {
                std::vector < std::string > addedWord; addedWord.resize(0);
                for(unsigned int i = 0; i < m_enterWord.size(); i++)
                {
                    std::string temp = "";
                    if (m_enterWord[i] > 'Z' || m_enterWord[i] < 'A') {temp+=m_enterWord[i]; temp+=m_enterWord[++i];}
                    else  temp+=m_enterWord[i];
                    addedWord.push_back(temp);
                }
                int madeScore = m_board->addWord(clickedTiles[0].first,clickedTiles[0].second,addedWord,m_enterOrientation,m_players[m_turn]->getLetters());

                if (madeScore) /* Succesfull added Word with non zero score */
                {
                    m_players[m_turn]->setScore( m_players[m_turn]->getScore() + madeScore);
                    m_scoreTable[m_turn][1]->updateText(std::to_string(m_players[m_turn]->getScore()));
                    nextTurn();
                }
            }
        }

        if (event.type == sf::Event::TextEntered) /* Typing players names */
        {
                if (m_enterWordButton->isPressed()){
                    if (event.text.unicode == '\b')
                    {
                        if (!m_enterWord.empty())
                        {
                            if (m_enterWord.size() >= 2 && (m_enterWord[m_enterWord.size()-1] > 'Z' || m_enterWord[m_enterWord.size()-1] < 'A')) {m_enterWord.pop_back(); m_enterWord.pop_back();}
                            else  m_enterWord.pop_back();
                            m_enterWordLength--;
                        }
                    }
                    else if (m_enterWordLength <= 15){m_enterWord+=GetCapital(UnicodeToUTF8(event.text.unicode)); if (GetCapital(UnicodeToUTF8(event.text.unicode)) != "") m_enterWordLength++;}
                    m_enterWordButton->updateText(m_enterWord);
                }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
                m_selectingLetters = false;
                m_enterWordButton->setPressed(false);
        }
    }
}
