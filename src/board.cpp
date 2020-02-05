#include"board.hpp"

Board::Board(sf::RenderWindow* window)
{
    /* Board texture */
    m_boardTexture.loadFromFile("static/plansza.png");
    m_boardTexture.setSmooth(true);
    m_boardSprite.setTexture(m_boardTexture);
    m_boardSprite.setScale(sf::Vector2f((float)window->getSize().y / (float)m_boardTexture.getSize().y / 1.4f, (float)window->getSize().y / (float)m_boardTexture.getSize().y / 1.4f));
    m_boardSprite.setPosition(sf::Vector2f(window->getSize().x / 2.f - (m_boardTexture.getSize().x * m_boardSprite.getScale().x) / 2.f, window->getSize().y / 10.f - 70));

    /* Board tables */
    for(int x=0; x<15; x++)
    {
        for(int y=0; y<15; y++)
        {
            m_lettersImg[x][y] = NULL;
            m_letterBonus[x][y] = 1;
            m_wordBonus[x][y] = 1;
            m_letters[x][y] = "";
            /* Setting words and letters bonus */
            if(x+y == 14 && abs(x-7) > 2) m_wordBonus[x][y] = 2;
            if(x==y && abs(x-7) > 2) m_wordBonus[x][y] = 2;
            if(x%7 == 0 && y%7 == 0) m_wordBonus[x][y] = 3;
        }
    }

    m_totalWords = 0;

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
    printf("[+] Reading dictionary from file. Please wait...\n"); 
    while (dictionaryFile >> word) m_dictionary[word] = 1;
    dictionaryFile.close();
}

Board::~Board()
{

}

void Board::draw(sf::RenderWindow* window)
{
    /* Drawing board and letters on it */
    window->draw(m_boardSprite);

    for(int x = 0; x < 15; x++)
        for(int y = 0; y < 15; y++)
            if(m_lettersImg[x][y]) window->draw(*m_lettersImg[x][y]->getSpritePointer());
}

int Board::addWord(int x, int y, std::vector < std::string > word, bool orientation, sf::RenderWindow* window)
{

    /* Debug */
    printf("\nTrying to put word \"");
    for(auto letter: word) std::cout<<letter;
    printf("\" on (%d,%d) cords orientated %d\n",x,y,orientation);
    /* End of debug */

    std::pair < bool, int > response = checkCorrectness(x,y,word,orientation);
    if(!response.first)
    {
        switch (response.second){
            case OUTREACHBOARD:
                printf("[-] Cannot put the word, it outreach the board.\n");
                break;
            case OVERWRITEWORD:
                printf("[-] Cannot put the word, it overwrites some word on board.\n");
                break;
            case UNCORRECTWORD:
                printf("[-] Cannot put the word, it's not correct.\n");
                break;
            case NONEIGHBOUR:
                printf("[-] Cannot put the word, no words in neighbourhood.\n");
                break;
        }

        return response.first;
    }

    /* Creating word on board, counting score */
    int score = countScore(x,y,word,orientation);
    if(orientation == VERTICAL)
    {
        for(unsigned int y_c = y; y_c < y + word.size(); y_c++)
        {
            m_lettersImg[x][y_c] = new Textbox(sf::Vector2f(window->getSize().x / 3.871f + x*window->getSize().x / 31.15f, window->getSize().y / 36.f + y_c*window->getSize().y / 21.12f), sf::Vector2i(window->getSize().x / 29.75f, window->getSize().y / 20.145f),"",0);
            m_lettersImg[x][y_c]->setImage(std::string("static/letters/pl/") + word[y_c-y] + ".png");
        }
    }
    if(orientation == HORIZONTAL)
    {
        for(unsigned int x_c = x; x_c < x + word.size(); x_c++)
        {
            m_lettersImg[x_c][y] = new Textbox(sf::Vector2f(window->getSize().x / 3.871f + x_c*window->getSize().x / 31.15f, window->getSize().y / 36.f + y*window->getSize().y / 21.12f), sf::Vector2i(window->getSize().x / 29.75f, window->getSize().y / 20.145f),"",0);
            m_lettersImg[x_c][y]->setImage(std::string("static/letters/pl/") + word[x_c-x] + ".png");
        }
    }

    /* Clear used bonus */
    if(orientation == VERTICAL) for(unsigned int y_c = y; y_c < y + word.size(); y_c++){m_letterBonus[x][y_c] = 1; m_wordBonus[x][y_c] = 1;}
    if(orientation == HORIZONTAL) for(unsigned int x_c = x; x_c < x + word.size(); x_c++){m_letterBonus[x_c][y] = 1; m_wordBonus[x_c][y] = 1;}

    m_totalWords++;
    printf("[+] Successfull added word with score %d!\n",score);

    return score;
}

int Board::countScore(int x, int y, std::vector < std::string > word, bool orientation)
{
    int score = 0;

    if(orientation == HORIZONTAL)
    {
        int currScore = 0; int currWordBonus = 1;
        for(int x_c = x-1; x_c >= 0 && m_letters[x_c][y] != ""; x_c--) {currScore += m_letterBonus[x_c][y] * m_lettersPoints[m_letters[x_c][y]]; currWordBonus *= m_wordBonus[x_c][y];}
        for(int x_c = x; x_c <= 14 && m_letters[x_c][y] != ""; x_c++) {currScore += m_letterBonus[x_c][y] * m_lettersPoints[m_letters[x_c][y]]; currWordBonus *= m_wordBonus[x_c][y];}
        score += currScore * currWordBonus;
        std::cout<<currScore<<" "<<currWordBonus<<"\n";
        currScore = 0; currWordBonus = 1;
        for(unsigned int x_c = x; x_c < x + word.size(); x_c++)
        {
            for(int y_c = y-1; y_c >= 0 && m_letters[x_c][y_c] != ""; y_c--) {currScore += m_letterBonus[x_c][y_c] * m_lettersPoints[m_letters[x_c][y_c]]; currWordBonus *= m_wordBonus[x_c][y_c];}
            for(int y_c = y; y_c <= 14 && m_letters[x_c][y_c] != ""; y_c++) {currScore += m_letterBonus[x_c][y_c] * m_lettersPoints[m_letters[x_c][y_c]]; currWordBonus *= m_wordBonus[x_c][y_c];}
            /* Delete extra points for same word */ if(y + 1 > 14 || m_letters[x_c][y+1] == "") {currScore -= m_letterBonus[x_c][y] * m_lettersPoints[m_letters[x_c][y]]; currWordBonus /= m_wordBonus[x_c][y];}
        }
        score += currScore * currWordBonus;
    }
    if(orientation == VERTICAL)
    {
        int currScore = 0; int currWordBonus = 1;
        for(int y_c = y-1; y_c >= 0 && m_letters[x][y_c] != ""; y_c--) {currScore += m_letterBonus[x][y_c] * m_lettersPoints[m_letters[x][y_c]]; currWordBonus *= m_wordBonus[x][y_c];}
        for(int y_c = y; y_c <= 14 && m_letters[x][y_c] != ""; y_c++) {currScore += m_letterBonus[x][y_c] * m_lettersPoints[m_letters[x][y_c]]; currWordBonus *= m_wordBonus[x][y_c];}
        score += currScore * currWordBonus;

        currScore = 0; currWordBonus = 1;
        for(unsigned int y_c = y; y_c < y + word.size(); y_c++)
        {
            for(int x_c = x-1; x_c >= 0 && m_letters[x_c][y_c] != ""; x_c--) {currScore += m_letterBonus[x_c][y_c] * m_lettersPoints[m_letters[x_c][y_c]]; currWordBonus *= m_wordBonus[x_c][y_c];}
            for(int x_c = x; x_c <= 14 && m_letters[x_c][y_c] != ""; x_c++) {currScore += m_letterBonus[x_c][y_c] * m_lettersPoints[m_letters[x_c][y_c]]; currWordBonus *= m_wordBonus[x_c][y_c];}
            /* Delete extra points for same word */ if(x + 1 > 14 || m_letters[x+1][y_c] == "") {currScore -= m_letterBonus[x][y_c] * m_lettersPoints[m_letters[x][y_c]]; currWordBonus /= m_wordBonus[x][y_c];}
        }
        score += currScore * currWordBonus;
    }

    return score;
}

std::pair < bool, int > Board::checkCorrectness(int x, int y, std::vector < std::string > word , bool orientation)
{
    /* Checking if the word fits in the board */
    if(x < 0 || x >= 15) return {false, OUTREACHBOARD};
    if(y < 0 || y >= 15) return {false, OUTREACHBOARD};

    if(orientation == VERTICAL)
        if(y + (int)word.size() > 15) return {false, OUTREACHBOARD};
    if(orientation == HORIZONTAL)
        if(x + (int)word.size() > 15) return {false, OUTREACHBOARD};

    if(orientation == VERTICAL) for(unsigned int y_c = y; y_c - y < word.size(); y_c++) if(m_letters[x][y_c] != "" && m_letters[x][y_c] != word[y_c-y]) return {false, OVERWRITEWORD};
    if(orientation == HORIZONTAL) for(unsigned int x_c = x; x_c - x < word.size(); x_c++) if(m_letters[x_c][y] != "" && m_letters[x_c][y] != word[x_c-x]) return {false, OVERWRITEWORD};

    /* Checking correctness of all new words */
    std::vector < std::string > newWords;
    newWords.resize(0);

    /* Checking NEW word in row/column */
    std::vector < std::string > prefix; std::vector < std::string > sufix;
    prefix.resize(0); sufix.resize(0);

    /* Check if word has a neighbour */
    bool temp = false;
    if(orientation == VERTICAL)
    {
        if(y>0 && m_letters[x][y-1] != "") temp = true;
        if(y+word.size() < 15 && m_letters[x][y+word.size()] != "") temp = true;
        for(unsigned int y_c = y; y_c - y < word.size(); y_c++) if(x - 1 >= 0 && m_letters[x-1][y_c] != "") temp = true;
        for(unsigned int y_c = y; y_c - y < word.size(); y_c++) if(x + 1 < 15 && m_letters[x+1][y_c] != "") temp = true;
    }
    if(orientation == HORIZONTAL)
    {
        if(x>0 && m_letters[x-1][y] != "") temp = true;
        if(x+word.size() < 15 && m_letters[x+word.size()][y] != "") temp = true;
        for(unsigned int x_c = x; x_c - x < word.size(); x_c++) if(y - 1 >= 0 && m_letters[x_c][y-1] != "") temp = true;
        for(unsigned int x_c = x; x_c - x < word.size(); x_c++) if(y + 1 < 15 && m_letters[x_c][y+1] != "") temp = true;
    }
    if(!temp && m_totalWords != 0) return {false, NONEIGHBOUR};


    /* Word fits the board, put it and check if NEW words are correct, if no take it back */
    if(orientation == VERTICAL) for(unsigned int y_c = y; y_c < y + word.size(); y_c++) m_letters[x][y_c] = word[y_c-y];
    if(orientation == HORIZONTAL) for(unsigned int x_c = x; x_c < x + word.size(); x_c++) m_letters[x_c][y] = word[x_c-x];


    getNewWord(x,y,orientation,&newWords);

    if(orientation == VERTICAL)
    {
        /* Checking NEW words in rows */
        for(unsigned int y_c = y; y_c < y + word.size(); y_c++)
            getNewWord(x,y_c,HORIZONTAL,&newWords);
    }
    if(orientation == HORIZONTAL)
    {
        /* Checking NEW words in columns */
        for(unsigned int x_c = x; x_c < x + word.size(); x_c++)
            getNewWord(x_c,y,VERTICAL,&newWords);
    }

    temp = true;
    for(auto newWord:newWords) 
        if(m_dictionary[newWord] != 1)
        {
            /* Take back the word from board if it's not correct */
            if(orientation == VERTICAL) for(unsigned int y_c = y; y_c < y + word.size(); y_c++) m_letters[x][y_c] = "";
            if(orientation == HORIZONTAL) for(unsigned int x_c = x; x_c < x + word.size(); x_c++) m_letters[x_c][y] = "";
            temp = false;
        }

    if(!temp) return {false, UNCORRECTWORD};
    return {true, CORRECTWORD};

}

void Board::getNewWord(int x, int y, bool orientation, std::vector < std::string >* newWords)
{
    std::vector < std::string > prefix; std::vector < std::string > sufix;
    prefix.resize(0); sufix.resize(0);

    if(orientation == VERTICAL)
    {
        for(int y_c = y-1; y_c >= 0 && m_letters[x][y_c] != ""; y_c--) prefix.push_back(m_letters[x][y_c]);
        for(int y_c = y; y_c <= 14 && m_letters[x][y_c] != ""; y_c++) sufix.push_back(m_letters[x][y_c]);
    }
    if(orientation == HORIZONTAL)
    {
        for(int x_c = x-1; x_c >= 0 && m_letters[x_c][y] != ""; x_c--) prefix.push_back(m_letters[x_c][y]);
        for(int x_c = x; x_c <= 14 && m_letters[x_c][y] != ""; x_c++) sufix.push_back(m_letters[x_c][y]);
    }
    std::reverse(prefix.begin(), prefix.end());
    std::string newWord = "";
    for(auto letter:prefix) newWord+=letter;
    for(auto letter:sufix) newWord+=letter;

    /* Skip simple newWords */
    if(prefix.size() + sufix.size() > 1) newWords->push_back(newWord);
}

void Board::debugRANDOMBOARD(sf::RenderWindow* window)
{
    srand(time(NULL));
    std::vector < std::string > letters = {"A","Ą","B","C","Ć","D","E","Ę","F","G","H","I","J","K","L","Ł","M","N","Ń","O","Ó","P","R","S","Ś","T","U","W","Y","Z","Ź","Ż"};
    int n = rand()%50+100;
    std::vector < std::string > randomwords[] = {
        {"A","L","E"},
        {"D"},
        {"D","U","P","A"},
        {"P","O","Z","D","R","A","W","I","A","M"},
        {"J", "A", "B", "Ł", "K", "O"},
        {"S", "L", "O", "N", "C", "E"},
        {"C", "Z", "O", "Ł", "G"},
        {"O", "B", "I", "A", "D"},
        {"O", "C", "H", "O", "T", "A"},
        {"N", "U", "D", "A"},
    };
    for(int i=0; i<n; i++){
        std::vector < std::string > randomword;
        randomword.resize(0);
        // int m =rand()%10+1;
        // for(int j=0; j<m; j++){
        //     randomword.push_back(letters[rand()%32]);
        // }
        randomword = randomwords[rand()%10];
        if(rand()%2)
            addWord(rand()%10,rand()%10,randomword,VERTICAL,window);
        else
            addWord(rand()%10,rand()%10,randomword,HORIZONTAL,window);
    }
}
