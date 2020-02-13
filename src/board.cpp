#include"board.hpp"

std::vector <std::string> convertString(std::string s)
{
    std::vector < std::string > v; v.resize(0);
    for(unsigned int i = 0; i < s.size(); i++)
    {
        std::string temp = "";
        if (s[i] > 'Z' || s[i] < 'A') {temp+=s[i]; temp+=s[++i];}
        else  temp+=s[i];
        v.push_back(temp);
    }

    return v;
}

Board::Board(sf::RenderWindow* window)
{
    /* Board texture */
    m_boardTexture.loadFromFile("static/board.png");
    m_boardTexture.setSmooth(true);
    m_boardSprite.setTexture(m_boardTexture);
    m_boardSprite.setScale(sf::Vector2f((float)window->getSize().y / (float)m_boardTexture.getSize().y / 1.4f, (float)window->getSize().y / (float)m_boardTexture.getSize().y / 1.4f));
    m_boardSprite.setPosition(sf::Vector2f(window->getSize().x / 2.f - (m_boardTexture.getSize().x * m_boardSprite.getScale().x) / 2.f, window->getSize().y / 10.f - 70));

    /* Board tables */
    for (int x=0; x<15; x++)
    {
        for (int y=0; y<15; y++)
        {
            m_lettersTiles[x][y] = new Button(sf::Vector2f(window->getSize().x / 3.871f + x*window->getSize().x / 31.15f, window->getSize().y / 36.f + y*window->getSize().y / 21.12f), sf::Vector2i(window->getSize().x / 29.75f, window->getSize().y / 20.145f),"",0);;
            m_letterBonus[x][y] = 1;
            m_wordBonus[x][y] = 1;
            m_letters[x][y] = "";
            /* Setting words and letters bonus */
            if (x+y == 14 && abs(x-7) > 2) m_wordBonus[x][y] = 2;
            if (x==y && abs(x-7) > 2) m_wordBonus[x][y] = 2;
            if (x%7 == 0 && y%7 == 0) m_wordBonus[x][y] = 3;
        }
    }

    m_totalWords = 0;
    m_allWords.resize(0);

    /* Setting words and letters bonus */
    m_wordBonus[7][7] = 2;
    m_letterBonus[5][1] = 3; m_letterBonus[9][1] = 3; m_letterBonus[1][5] = 3; m_letterBonus[5][5] = 3; m_letterBonus[9][5] = 3; m_letterBonus[13][5] = 3; m_letterBonus[1][9] = 3; m_letterBonus[5][9] = 3; m_letterBonus[9][9] = 3; m_letterBonus[13][9] = 3; m_letterBonus[5][13] = 3; m_letterBonus[9][13] = 3;
    m_letterBonus[3][0] = 2; m_letterBonus[11][0] = 2; m_letterBonus[3][7] = 2; m_letterBonus[11][7] = 2; m_letterBonus[3][14] = 2; m_letterBonus[11][14] = 2; m_letterBonus[6][2] = 2; m_letterBonus[8][2] = 2; m_letterBonus[6][12] = 2; m_letterBonus[8][12] = 2; m_letterBonus[0][3] = 2; m_letterBonus[7][3] = 2; m_letterBonus[14][3] = 2; m_letterBonus[0][11] = 2; m_letterBonus[7][11] = 2; m_letterBonus[14][11] = 2;
    m_letterBonus[2][6] = 2; m_letterBonus[6][6] = 2; m_letterBonus[8][6] = 2; m_letterBonus[12][6] = 2;m_letterBonus[2][8] = 2; m_letterBonus[6][8] = 2; m_letterBonus[8][8] = 2; m_letterBonus[12][8] = 2;

    /* Polish letters points */
    m_lettersPoints["_"] = 0;
    m_lettersPoints["A"] = 1; m_lettersPoints["E"] = 1; m_lettersPoints["I"] = 1; m_lettersPoints["N"] = 1; m_lettersPoints["O"] = 1; m_lettersPoints["R"] = 1; m_lettersPoints["S"] = 1; m_lettersPoints["W"] = 1; m_lettersPoints["Z"] = 1;
    m_lettersPoints["C"] = 2; m_lettersPoints["D"] = 2; m_lettersPoints["K"] = 2; m_lettersPoints["L"] = 2; m_lettersPoints["M"] = 2; m_lettersPoints["P"] = 2; m_lettersPoints["T"] = 2; m_lettersPoints["Y"] = 2;
    m_lettersPoints["B"] = 3; m_lettersPoints["G"] = 3; m_lettersPoints["H"] = 3; m_lettersPoints["J"] = 3; m_lettersPoints["Ł"] = 3; m_lettersPoints["U"] = 3;
    m_lettersPoints["Ą"] = 5; m_lettersPoints["Ę"] = 5; m_lettersPoints["F"] = 5; m_lettersPoints["Ó"] = 5; m_lettersPoints["Ś"] = 5; m_lettersPoints["Ż"] = 5;
    m_lettersPoints["Ń"] = 7;
    m_lettersPoints["Ź"] = 9;

    /* Reading dictionary from file */
    std::fstream dictionaryFile; std::string word;
    dictionaryFile.open("static/polish_words.txt");
    NOBRUT printf("[+] Reading dictionary from file. Please wait...\n"); 
    while (dictionaryFile >> word) m_dictionary[word] = 1;
    dictionaryFile.close();

    m_alphabet =  {"A","Ą","B","C","Ć","D","E","Ę","F","G","H","I","J","K","L","Ł","M","N","Ń","O","Ó","P","R","S","Ś","T","U","W","Y","Z","Ź","Ż"};

    m_makingBestMove = 0;
    m_addingWord = 0;
}

Board::~Board()
{

}

void Board::drawWordHover(sf::RenderWindow* window, WordOnBoard word)
{
    /* Drawing board and letters on it */
    window->draw(m_boardSprite);

    std::vector <std::string> wordV;
    wordV = convertString(word.word);
    /* No word Hover */
    if (word.x == -1) return;
    if (word.orientation == VERTICAL)
        for (int y_c = word.y; y_c < std::min(word.y + (int)wordV.size(),15); y_c++)
        {
            m_lettersTiles[word.x][y_c]->setImage(std::string("static/letters/pl/") + wordV[y_c-word.y] + ".png");
            m_lettersTiles[word.x][y_c]->setSpriteColor(sf::Color(125,125,126));
            window->draw(*m_lettersTiles[word.x][y_c]->getSpritePointer());
        }
    else
        for (int x_c = word.x; x_c < std::min(word.x + (int)wordV.size(),15); x_c++)
        {
            m_lettersTiles[x_c][word.y]->setImage(std::string("static/letters/pl/") + wordV[x_c-word.x] + ".png");
            m_lettersTiles[x_c][word.y]->setSpriteColor(sf::Color(125,125,126));
            window->draw(*m_lettersTiles[x_c][word.y]->getSpritePointer());
        }
}

void Board::draw(sf::RenderWindow* window)
{
    for (int x = 0; x < 15; x++)
        for (int y = 0; y < 15; y++)
            if (m_lettersTiles[x][y] && m_letters[x][y] != "")
            {
                /* Clean up */
                m_lettersTiles[x][y]->setImage(std::string("static/letters/pl/") + m_letters[x][y] + ".png");
                m_lettersTiles[x][y]->setSpriteColor(sf::Color(255,255,255));

                window->draw(*m_lettersTiles[x][y]->getSpritePointer());
            }
}

bool Board::checkTilePress(int x, int y, sf::Vector2i mousePosition)
{
    return m_lettersTiles[x][y]->check(mousePosition);
}

std::pair <int, std::vector <int> > Board::checkWord(Move move, std::vector < std::string > playersLetters)
{
    NOBRUT printf("\nChecking word \"");
    for (auto letter: move.word) NOBRUT printf("%s",letter.c_str());
    NOBRUT printf("\" on (%d,%d) cords orientated %d\n",move.x,move.y,move.orientation);

    std::pair < bool, std::pair <int , std::vector <int> > > response = checkCorrectness(move,playersLetters);
    std::vector <int> blanks = response.second.second;
    if (!response.first)
    {
        switch (response.second.first){
            case OUTREACHBOARD:
                NOBRUT printf("[-] Cannot put the word, it outreach the board.\n");
                break;
            case OVERWRITEWORD:
                NOBRUT printf("[-] Cannot put the word, it overwrites some word on board.\n");
                break;
            case UNCORRECTWORD:
                NOBRUT printf("[-] Cannot put the word, it's not correct.\n");
                break;
            case NONEIGHBOUR:
                NOBRUT printf("[-] Cannot put the word, no words in neighbourhood.\n");
                break;
            case MISSINGLETTERS:
                NOBRUT printf("[-] Missing some letters.\n");
                break;
            case SAMEWORD:
                NOBRUT printf("[-] It is not a new word.\n");
                break;
            case NOMIDDLEFIRSTWORD:
                NOBRUT printf("[-] First word must be on middle.\n");
                break;
        }

        return {-1,blanks};
    }

    /* Creating word on board, counting score */
    std::string m_lettersOG[15][15];
    for (int x = 0; x < 15; x++)
        for (int y = 0; y < 15; y++)
            m_lettersOG[x][y] = m_letters[x][y];
    if (move.orientation == VERTICAL) for (unsigned int y_c = move.y; y_c < move.y + move.word.size(); y_c++) m_letters[move.x][y_c] = move.word[y_c-move.y];
    if (move.orientation == HORIZONTAL) for (unsigned int x_c = move.x; x_c < move.x + move.word.size(); x_c++) m_letters[x_c][move.y] = move.word[x_c-move.x];
    int score = countScore(move,blanks);
    /* Take word back if only checkinbg */
    if (!m_addingWord)
    {
        if (move.orientation == VERTICAL) for (unsigned int y_c = move.y; y_c < move.y + move.word.size(); y_c++) m_letters[move.x][y_c] = m_lettersOG[move.x][y_c];
        if (move.orientation == HORIZONTAL) for (unsigned int x_c = move.x; x_c < move.x + move.word.size(); x_c++) m_letters[x_c][move.y] = m_lettersOG[x_c][move.y];
    }

    /* Adding all letters bonud */
    if(response.second.first == CORRECTWORD50BONUS) {score+=50; NOBRUT printf("[+] All letters used = 50 points bonus!\n");}
    NOBRUT printf("[+] Successfull added word with score %d!\n",score);

    return {score,blanks};

}
int Board::addWord(Move move, std::vector < std::string > playersLetters)
{
    m_addingWord = 1;

    NOBRUT printf("\nTrying to put word \"");
    for (auto letter: move.word) NOBRUT printf("%s",letter.c_str());
    NOBRUT printf("\" on (%d,%d) cords orientated %d\n",move.x,move.y,move.orientation);

    /* Creating word on board, counting score */
    std::pair <int, std::vector <int> > response = checkWord(move,playersLetters);
    std::vector <int> blanks = response.second;
    /* Unsuccessfull attend to add word */
    if (response.first == -1)
    {
        NOBRUT printf("[-] Cannot put word.\n");
        m_addingWord = 0;
        return -1;
    }
    if (move.orientation == VERTICAL)
    {
        for (unsigned int y_c = move.y; y_c < move.y + move.word.size(); y_c++)
            if (m_letterBonus[move.x][y_c]) m_lettersTiles[move.x][y_c]->setImage(std::string("static/letters/pl/") + move.word[y_c-move.y] + ".png");
        for (auto blank: blanks) m_lettersTiles[move.x][move.y+blank]->setImage(std::string("static/letters/pl/") + "_" + ".png");
    }
    if (move.orientation == HORIZONTAL)
    {
        for (unsigned int x_c = move.x; x_c < move.x + move.word.size(); x_c++)
            if (m_letterBonus[x_c][move.y]) m_lettersTiles[x_c][move.y]->setImage(std::string("static/letters/pl/") + move.word[x_c-move.x] + ".png");
        for (auto blank: blanks) m_lettersTiles[move.x+blank][move.y]->setImage(std::string("static/letters/pl/") + "_" + ".png");
    }

    /* Clear used bonus */
    if (move.orientation == VERTICAL)
    {
        for (unsigned int y_c = move.y; y_c < move.y + move.word.size(); y_c++){if (m_letterBonus[move.x][y_c]) m_letterBonus[move.x][y_c] = 1; m_wordBonus[move.x][y_c] = 1;}
        for (auto blank: blanks) m_letterBonus[move.x][move.y+blank] = 0;
    }
    if (move.orientation == HORIZONTAL)
    {
        for (unsigned int x_c = move.x; x_c < move.x + move.word.size(); x_c++){if (m_letterBonus[x_c][move.y]) m_letterBonus[x_c][move.y] = 1; m_wordBonus[x_c][move.y] = 1;}
        for (auto blank: blanks) m_letterBonus[move.x+blank][move.y] = 0;
    }
    m_totalWords++;

    m_addingWord = 0;
    return response.first;
}

int Board::countScore(Move move, std::vector <int> blanks)
{
    int score = 0;

    if (move.orientation == HORIZONTAL)
    {
        /* Blanks gives 0 points */
        for(auto blank: blanks) m_letterBonus[move.x+blank][move.y] = 0;

        int currScore = 0; int currWordBonus = 1;
        for (int x_c = move.x-1; x_c >= 0 && m_letters[x_c][move.y] != ""; x_c--) {currScore += m_letterBonus[x_c][move.y] * m_lettersPoints[m_letters[x_c][move.y]]; currWordBonus *= m_wordBonus[x_c][move.y];}
        for (int x_c = move.x; x_c <= 14 && m_letters[x_c][move.y] != ""; x_c++) {currScore += m_letterBonus[x_c][move.y] * m_lettersPoints[m_letters[x_c][move.y]]; currWordBonus *= m_wordBonus[x_c][move.y];}
        score += currScore * currWordBonus;

        currScore = 0; currWordBonus = 1;
        for (unsigned int x_c = move.x; x_c < move.x + move.word.size(); x_c++)
        {
            if(m_letters[x_c][move.y] == "") /* Add only new words */
            {
                for (int y_c = move.y-1; y_c >= 0 && m_letters[x_c][y_c] != ""; y_c--) {currScore += m_letterBonus[x_c][y_c] * m_lettersPoints[m_letters[x_c][y_c]]; currWordBonus *= m_wordBonus[x_c][y_c];}
                for (int y_c = move.y; y_c <= 14 && m_letters[x_c][y_c] != ""; y_c++) {currScore += m_letterBonus[x_c][y_c] * m_lettersPoints[m_letters[x_c][y_c]]; currWordBonus *= m_wordBonus[x_c][y_c];}
                /* Delete extra points for same word */ if (move.y + 1 > 14 || m_letters[x_c][move.y+1] == "") {currScore -= m_letterBonus[x_c][move.y] * m_lettersPoints[m_letters[x_c][move.y]]; currWordBonus /= m_wordBonus[x_c][move.y];}
            }
        }
        score += currScore * currWordBonus;
    }
    if (move.orientation == VERTICAL)
    {
        /* Blanks gives 0 points */
        for(auto blank: blanks) m_letterBonus[move.x][move.y+blank] = 0;

        int currScore = 0; int currWordBonus = 1;
        for (int y_c = move.y-1; y_c >= 0 && m_letters[move.x][y_c] != ""; y_c--) {currScore += m_letterBonus[move.x][y_c] * m_lettersPoints[m_letters[move.x][y_c]]; currWordBonus *= m_wordBonus[move.x][y_c];}
        for (int y_c = move.y; y_c <= 14 && m_letters[move.x][y_c] != ""; y_c++) {currScore += m_letterBonus[move.x][y_c] * m_lettersPoints[m_letters[move.x][y_c]]; currWordBonus *= m_wordBonus[move.x][y_c];}
        score += currScore * currWordBonus;

        currScore = 0; currWordBonus = 1;
        for (unsigned int y_c = move.y; y_c < move.y + move.word.size(); y_c++)
        {
            if(m_letters[move.x][y_c] == "") /* Add only new words */
            {
                for (int x_c = move.x-1; x_c >= 0 && m_letters[x_c][y_c] != ""; x_c--) {currScore += m_letterBonus[x_c][y_c] * m_lettersPoints[m_letters[x_c][y_c]]; currWordBonus *= m_wordBonus[x_c][y_c];}
                for (int x_c = move.x; x_c <= 14 && m_letters[x_c][y_c] != ""; x_c++) {currScore += m_letterBonus[x_c][y_c] * m_lettersPoints[m_letters[x_c][y_c]]; currWordBonus *= m_wordBonus[x_c][y_c];}
                /* Delete extra points for same word */ if (move.x + 1 > 14 || m_letters[move.x+1][y_c] == "") {currScore -= m_letterBonus[move.x][y_c] * m_lettersPoints[m_letters[move.x][y_c]]; currWordBonus /= m_wordBonus[move.x][y_c];}
            }
        }
        score += currScore * currWordBonus;
    }

    return score;
}

std::pair < bool, std::pair < int, std::vector <int> > > Board::checkCorrectness(Move move, std::vector < std::string > playersLetters)
{
    bool isNeighbour = false;
    int numPlayerLettersOG = playersLetters.size();
    std::vector <int> usedBlanks; usedBlanks.resize(0);
    /* Checking if the word fits in the board */
    if (move.x < 0 || move.x >= 15) return {false, {OUTREACHBOARD, usedBlanks}};
    if (move.y < 0 || move.y >= 15) return {false, {OUTREACHBOARD, usedBlanks}};

    if (move.orientation == VERTICAL)
        if (move.y + (int)move.word.size() > 15) return {false, {OUTREACHBOARD, usedBlanks}};
    if (move.orientation == HORIZONTAL)
        if (move.x + (int)move.word.size() > 15) return {false, {OUTREACHBOARD, usedBlanks}};

    if (move.orientation == VERTICAL) for (unsigned int y_c = move.y; y_c - move.y < move.word.size(); y_c++) if (m_letters[move.x][y_c] != "" && m_letters[move.x][y_c] != move.word[y_c-move.y]) return {false, {OVERWRITEWORD, usedBlanks}};
    if (move.orientation == HORIZONTAL) for (unsigned int x_c = move.x; x_c - move.x < move.word.size(); x_c++) if (m_letters[x_c][move.y] != "" && m_letters[x_c][move.y] != move.word[x_c-move.x]) return {false, {OVERWRITEWORD, usedBlanks}};

    int numPlayerLetters = playersLetters.size();
    /* Check if player have enough letters */
    if (move.orientation == VERTICAL)
        for (unsigned int y_c = move.y; y_c - move.y < move.word.size(); y_c++)
        {
            if(m_letters[move.x][y_c] == "")
            {
                bool find = 0;
                for (unsigned int i = 0; i < playersLetters.size(); i++)
                {
                    if(move.word[y_c-move.y] == playersLetters[i])
                    {
                        find = 1;
                        playersLetters.erase(playersLetters.begin() + i);
                        break;
                    }
                }
                if (!find)
                {
                    for (unsigned int i = 0; i < playersLetters.size(); i++)
                    {
                        if (playersLetters[i] == "_")
                        {
                            find = 1;
                            playersLetters.erase(playersLetters.begin() + i);
                            usedBlanks.push_back(y_c-move.y);
                            break;
                        }
                    }
                }
                if (!find) return {false, {MISSINGLETTERS, usedBlanks}};
            }
            else isNeighbour = true;
        }
    if (move.orientation == HORIZONTAL)
        for (unsigned int x_c = move.x; x_c - move.x < move.word.size(); x_c++)
        {
            if(m_letters[x_c][move.y] == "")
            {
                bool find = 0;
                for (unsigned int i = 0; i < playersLetters.size(); i++)
                {
                    if(move.word[x_c-move.x] == playersLetters[i] || playersLetters[i] == "_")
                    {
                        find = 1;
                        playersLetters.erase(playersLetters.begin() + i);
                        break;
                    }
                }
                if (!find)
                {
                    for (unsigned int i = 0; i < playersLetters.size(); i++)
                    {
                        if (playersLetters[i] == "_")
                        {
                            find = 1;
                            playersLetters.erase(playersLetters.begin() + i);
                            usedBlanks.push_back(x_c-move.x);
                            break;
                        }
                    }
                }
                if (!find) return {false, {MISSINGLETTERS, usedBlanks}};
            }
            else isNeighbour = true;
        }

    if ((int)playersLetters.size() == numPlayerLetters) return {false, {SAMEWORD, usedBlanks}};

    /* Checking correctness of all new words */
    std::vector <WordOnBoard> newWords;
    newWords.resize(0);

    /* Checking NEW word in row/column */
    std::vector < std::string > prefix; std::vector < std::string > sufix;
    prefix.resize(0); sufix.resize(0);

    /* Check if word has a neighbour */
    if (move.orientation == VERTICAL)
    {
        if (move.y>0 && m_letters[move.x][move.y-1] != "") isNeighbour = true;
        if (move.y+move.word.size() < 15 && m_letters[move.x][move.y+move.word.size()] != "") isNeighbour = true;
        for (unsigned int y_c = move.y; y_c - move.y < move.word.size(); y_c++) if (move.x - 1 >= 0 && m_letters[move.x-1][y_c] != "") isNeighbour = true;
        for (unsigned int y_c = move.y; y_c - move.y < move.word.size(); y_c++) if (move.x + 1 < 15 && m_letters[move.x+1][y_c] != "") isNeighbour = true;
    }
    if (move.orientation == HORIZONTAL)
    {
        if (move.x>0 && m_letters[move.x-1][move.y] != "") isNeighbour = true;
        if (move.x+move.word.size() < 15 && m_letters[move.x+move.word.size()][move.y] != "") isNeighbour = true;
        for (unsigned int x_c = move.x; x_c - move.x < move.word.size(); x_c++) if (move.y - 1 >= 0 && m_letters[x_c][move.y-1] != "") isNeighbour = true;
        for (unsigned int x_c = move.x; x_c - move.x < move.word.size(); x_c++) if (move.y + 1 < 15 && m_letters[x_c][move.y+1] != "") isNeighbour = true;
    }
    if (!isNeighbour && m_totalWords != 0) return {false, {NONEIGHBOUR, usedBlanks}};

    if (m_totalWords == 0)
    {
        bool isMiddle = false;
        if (move.orientation == VERTICAL) for (unsigned int y_c = move.y; y_c < move.y + move.word.size(); y_c++) if ( move.x == 7 && y_c == 7) isMiddle = true;
        if (move.orientation == HORIZONTAL) for (unsigned int x_c = move.x; x_c < move.x + move.word.size(); x_c++) if ( x_c == 7 && move.y == 7) isMiddle = true;
        if (!isMiddle) return {false, {NOMIDDLEFIRSTWORD, usedBlanks}};
    }
    /* Word fits the board, put it and check if NEW words are correct, if no take it back */
    std::string m_lettersOG[15][15];
    for (int x = 0; x < 15; x++)
        for (int y = 0; y < 15; y++)
            m_lettersOG[x][y] = m_letters[x][y];
    if (move.orientation == VERTICAL) for (unsigned int y_c = move.y; y_c < move.y + move.word.size(); y_c++) m_letters[move.x][y_c] = move.word[y_c-move.y];
    if (move.orientation == HORIZONTAL) for (unsigned int x_c = move.x; x_c < move.x + move.word.size(); x_c++) m_letters[x_c][move.y] = move.word[x_c-move.x];

    getNewWord(move.x,move.y,move.orientation,&newWords,false);

    if (move.orientation == VERTICAL)
    {
        /* Getting NEW words in rows */
        for (unsigned int y_c = move.y; y_c < move.y + move.word.size(); y_c++)
            getNewWord(move.x,y_c,HORIZONTAL,&newWords,false);
    }
    if (move.orientation == HORIZONTAL)
    {
        /* Getting NEW words in columns */
        for (unsigned int x_c = move.x; x_c < move.x + move.word.size(); x_c++)
            getNewWord(x_c,move.y,VERTICAL,&newWords,false);
    }

    bool isCorrect = true;

    for (auto newWord: newWords)
        if (m_dictionary[newWord.word] != 1)
        {
            /* Take back the word from board if it's not correct */
            if (move.orientation == VERTICAL) for (unsigned int y_c = move.y; y_c < move.y + move.word.size(); y_c++) m_letters[move.x][y_c] = m_lettersOG[move.x][y_c];
            if (move.orientation == HORIZONTAL) for (unsigned int x_c = move.x; x_c < move.x + move.word.size(); x_c++) m_letters[x_c][move.y] = m_lettersOG[x_c][move.y];
            isCorrect = false;
        }

    if (!isCorrect) return {false, {UNCORRECTWORD, usedBlanks}};

    if(m_addingWord)
    {
        /* Getting NEW words*/
        getNewWord(move.x,move.y,move.orientation,&m_allWords,true);
        if (move.orientation == VERTICAL)
        {
            for (unsigned int y_c = move.y; y_c < move.y + move.word.size(); y_c++)
                getNewWord(move.x,y_c,HORIZONTAL,&m_allWords,true);
        }
        if (move.orientation == HORIZONTAL)
        {
            /* Getting NEW words in columns */
            for (unsigned int x_c = move.x; x_c < move.x + move.word.size(); x_c++)
                getNewWord(x_c,move.y,VERTICAL,&m_allWords,true);
        }
    }

    /* Take word back and put it in checkWord in adding */
    if (move.orientation == VERTICAL) for (unsigned int y_c = move.y; y_c < move.y + move.word.size(); y_c++) m_letters[move.x][y_c] = m_lettersOG[move.x][y_c];
    if (move.orientation == HORIZONTAL) for (unsigned int x_c = move.x; x_c < move.x + move.word.size(); x_c++) m_letters[x_c][move.y] = m_lettersOG[x_c][move.y];

    if (!playersLetters.size() && numPlayerLettersOG == 7) return {true, {CORRECTWORD50BONUS, usedBlanks}};
    return {true, {CORRECTWORD, usedBlanks}};

}

void Board::getNewWord(int x, int y, bool orientation, std::vector <WordOnBoard>* newWords,bool newWordFlag)
{
    std::vector < std::string > prefix; std::vector < std::string > sufix;
    prefix.resize(0); sufix.resize(0);

    if (orientation == VERTICAL)
    {
        for (int y_c = y-1; y_c >= 0 && m_letters[x][y_c] != ""; y_c--) prefix.push_back(m_letters[x][y_c]);
        for (int y_c = y; y_c <= 14 && m_letters[x][y_c] != ""; y_c++) sufix.push_back(m_letters[x][y_c]);
    }
    if (orientation == HORIZONTAL)
    {
        for (int x_c = x-1; x_c >= 0 && m_letters[x_c][y] != ""; x_c--) prefix.push_back(m_letters[x_c][y]);
        for (int x_c = x; x_c <= 14 && m_letters[x_c][y] != ""; x_c++) sufix.push_back(m_letters[x_c][y]);
    }
    std::reverse(prefix.begin(), prefix.end());
    WordOnBoard newWord;
    newWord.x = x; newWord.y = y; newWord.orientation = orientation; newWord.word = "";
    for (auto letter:prefix) newWord.word+=letter;
    for (auto letter:sufix) newWord.word+=letter;

    /* Skip simple newWords */
    if (!newWordFlag && prefix.size() + sufix.size() > 1)
        newWords->push_back(newWord);
    /* Getting all new words */
    if (newWordFlag)
    {
        /* Do not duplicate new words */
        bool flag = 1;
        for (unsigned int i = 0; i < newWords->size(); i++)
        {
            if ((*newWords)[i].x == newWord.x && (*newWords)[i].y == newWord.y && (*newWords)[i].orientation == newWord.orientation && (*newWords)[i].word == newWord.word) flag = 0;
        }
        if (flag) newWords->push_back(newWord);
    }
}

std::vector <std::string> Board::unusedLetters(Move move, std::vector < std::string > playersLetters)
{
    std::vector <std::string> neededLetters; neededLetters.resize(0);
    if (move.orientation == VERTICAL)
        for (unsigned int y_c = move.y; y_c - move.y < move.word.size(); y_c++)
        {
            if(y_c < 15 && m_letters[move.x][y_c] == "")
            {
                bool find = 0;
                for (unsigned int i = 0; i < playersLetters.size(); i++)
                {
                    if(move.word[y_c-move.y] == playersLetters[i])
                    {
                        find = 1;
                        neededLetters.push_back(playersLetters[i]);
                        break;
                    }
                }
                if (!find)
                {
                    for (unsigned int i = 0; i < playersLetters.size(); i++)
                    {
                        if (playersLetters[i] == "_")
                        {
                            find = 1;
                            neededLetters.push_back(playersLetters[i]);
                            break;
                        }
                    }
                }
            }
        }
    if (move.orientation == HORIZONTAL)
        for (unsigned int x_c = move.x; x_c - move.x < move.word.size(); x_c++)
        {
            if(x_c < 15 && m_letters[x_c][move.y] == "")
            {
                bool find = 0;
                for (unsigned int i = 0; i < playersLetters.size(); i++)
                {
                    if(move.word[x_c-move.x] == playersLetters[i] || playersLetters[i] == "_")
                    {
                        find = 1;
                        neededLetters.push_back(playersLetters[i]);
                        break;
                    }
                }
                if (!find)
                {
                    for (unsigned int i = 0; i < playersLetters.size(); i++)
                    {
                        if (playersLetters[i] == "_")
                        {
                            find = 1;
                            neededLetters.push_back(playersLetters[i]);
                            break;
                        }
                    }
                }
            }
        }

    std::vector <std::string> noneededLetters; noneededLetters.resize(0);
    for (auto letter: playersLetters) noneededLetters.push_back(letter);
    for (auto neededLetter: neededLetters)
    {
        for (unsigned int i = 0; i < noneededLetters.size(); i++)
        {
            if (noneededLetters[i] == neededLetter) {noneededLetters.erase(noneededLetters.begin() + i); break;}
        }
    }
    return noneededLetters;
}

Move Board::getBestMove(std::vector < std::string > playersLetters)
{
    Move bestMove {0,0,VERTICAL,{},0};
    Move candidate {0,0,VERTICAL,{},0};

    m_makingBestMove = 1;

    if (m_totalWords == 0)
    {
        printf("[+] No words on the board\n");
        WordOnBoard temp;
        temp.x = 7; temp.y = 6; temp.orientation = VERTICAL;
        temp.word = {""};
        m_allWords.push_back(temp);
    }

    int counter = 0;
    /* Brute force ;) */
    int numLetters = playersLetters.size();
    for (unsigned int it = 0; it < m_allWords.size(); it++)
    {
        WordOnBoard existingWord = m_allWords[it];
        for (int mask = 1; mask < (1<<numLetters); mask++)
        {
            std::vector <std::string> usedLetters; usedLetters.resize(0); /* Generating masks */
            for (int i = 0; i < numLetters; i++) if ((1<<i)&mask) usedLetters.push_back(playersLetters[i]);

            int numUsedLetters = usedLetters.size();
            if (numUsedLetters <= 7){
            do {
                for (int i = 0; i<=numUsedLetters; i++)
                {
                    candidate.orientation = existingWord.orientation;
                    if (existingWord.orientation == VERTICAL) {candidate.y = existingWord.y - i; candidate.x = existingWord.x;}
                    if (existingWord.orientation == HORIZONTAL) {candidate.x = existingWord.x - i; candidate.y = existingWord.y;}

                     std::string wordToCheck = "";
                     for (int prefix = 0; prefix < i; prefix++)
                         wordToCheck+=usedLetters[prefix];
                     for (auto letterExistingWord: existingWord.word) wordToCheck+=letterExistingWord;
                     for (int sufix = 0; sufix < numUsedLetters - i; sufix++)
                         wordToCheck+=usedLetters[i+sufix];
                     candidate.word = convertString(wordToCheck);

                    /* Blank can be everything */
                    bool oneBlank = 0;
                    for (int i = 0; i < numUsedLetters; i++)
                    {
                        if (usedLetters[i] == "_")
                        {
                            oneBlank = 1;
                            bool twoBlanks = 0;
                            for (auto letterNoBlank1: m_alphabet)
                            {
                                for (unsigned int t = 0; t < candidate.word.size(); t++)
                                    if (candidate.word[t] == "_") candidate.word[t] = letterNoBlank1;
                                for (int j = 1; j < numUsedLetters; j++)
                                {
                                    if (usedLetters[j] == "_")
                                    {
                                        twoBlanks = 1;
                                        for (auto letterNoBlank2: m_alphabet)
                                        {
                                            for (unsigned int t = 0; t < candidate.word.size(); t++)
                                                if (candidate.word[t] == "_") candidate.word[t] = letterNoBlank2;
                                            candidate.score = checkWord(candidate, usedLetters).first;
                                            counter++;
                                            if(counter%50000 == 0) printf("[+] Bruting. Tried %d words. Best current score is %d.\n",counter, bestMove.score);
                                            if(candidate.score > bestMove.score) bestMove = candidate;
                                        }
                                    }
                                }

                                if (!twoBlanks)
                                {
                                    candidate.score = checkWord(candidate, usedLetters).first;
                                    if(candidate.score > bestMove.score) bestMove = candidate;
                                    counter++;
                                    if(counter%50000 == 0) printf("[+] Bruting. Tried %d words. Best current score is %d.\n",counter, bestMove.score);
                                }
                            }
                        }
                    }
                    /* No blanks */
                    if (!oneBlank)
                    {
                        candidate.score = checkWord(candidate, usedLetters).first;
                        if(candidate.score > bestMove.score) bestMove = candidate;
                        counter++;
                        if(counter%50000 == 0) printf("[+] Bruting. Tried %d words. Best current score is %d.\n",counter, bestMove.score);
                    }
                }
            } while (std::next_permutation(usedLetters.begin(), usedLetters.end()));
            }
        }
    }

    printf("[+] Best move is: "); for (auto letter: bestMove.word) printf("%s",letter.c_str()); printf(" on (%d,%d) cords with score %d.\n",bestMove.x,bestMove.y,bestMove.score);

    m_makingBestMove = 0;
    return bestMove;

}

int Board::summLetters(std::vector <std::string> letters)
{
    int summ = 0;
    for (unsigned int i = 0; i < letters.size(); i++)
    {
        summ+=m_lettersPoints[letters[i]];
    }
    return summ;
}