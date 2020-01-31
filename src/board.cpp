#include"board.hpp"

Board::Board()
{
    for(int x=0; x<15; x++)
    {
        for(int y=0; y<15; y++)
        {
            m_letters[x][y] = NULL;
            m_letterBonus[x][y] = 1;
            m_wordBonus[x][y] = 1;
            /* Setting words and letters bonus */
            if(x+y == 14 && abs(x-7) > 2) m_wordBonus[x][y] = 2;
            if(x==y && abs(x-7) > 2) m_wordBonus[x][y] = 2;
            if(x%7 == 0 && y%7 == 0) m_wordBonus[x][y] = 3;
        }
    }
    /* Setting words and letters bonus */
    m_wordBonus[7][7] = 2;
    m_letterBonus[5][1] = 3; m_letterBonus[9][1] = 3; m_letterBonus[1][5] = 3; m_letterBonus[5][5] = 3; m_letterBonus[9][5] = 3; m_letterBonus[13][5] = 3; m_letterBonus[1][9] = 3; m_letterBonus[5][9] = 3; m_letterBonus[9][9] = 3; m_letterBonus[13][9] = 3; m_letterBonus[5][13] = 3; m_letterBonus[9][13] = 3;
    m_letterBonus[3][0] = 2; m_letterBonus[11][0] = 2; m_letterBonus[3][7] = 2; m_letterBonus[11][7] = 2; m_letterBonus[3][14] = 2; m_letterBonus[11][14] = 2; m_letterBonus[6][2] = 2; m_letterBonus[8][2] = 2; m_letterBonus[6][12] = 2; m_letterBonus[8][12] = 2; m_letterBonus[0][3] = 2; m_letterBonus[7][3] = 2; m_letterBonus[14][3] = 2; m_letterBonus[0][11] = 2; m_letterBonus[7][11] = 2; m_letterBonus[14][11] = 2;
    m_letterBonus[2][6] = 2; m_letterBonus[6][6] = 2; m_letterBonus[8][6] = 2; m_letterBonus[12][6] = 2;m_letterBonus[2][8] = 2; m_letterBonus[6][8] = 2; m_letterBonus[8][8] = 2; m_letterBonus[12][8] = 2;
}

Board::~Board()
{

}

int Board::addWord(int x, int y, std::string word, bool orientation, sf::RenderWindow* window)
{
    /* Checking if the word fits in the board */
    if(x < 0 || x >= 15) return -1;
    if(y < 0 || y >= 15) return -1;
    if(orientation==VERTICAL)
        if(y + (int)word.size() >= 15) return -1;
    else
        if(x + (int)word.size() >= 15) return -1;
    /* Creating word on board, counting score */
    int score = 0;
    if(orientation==VERTICAL)
    {
        for(unsigned int y_c = y; y_c < y + word.size(); y_c++)
        {
            m_letters[x][y_c] = new Textbox(sf::Vector2f(window->getSize().x / 3.871f, window->getSize().y / 36.f + y_c*window->getSize().y / 21.12f), sf::Vector2i(window->getSize().x / 29.75f, window->getSize().y / 20.145f),"",0);
            m_letters[x][y_c]->setImage(std::string("static/letters/pl/") + word[y_c-y] + ".png");
        }
    }
    else
    {
        for(unsigned int x_c = x; x_c < x + word.size(); x_c++)
        {
            m_letters[x_c][y] = new Textbox(sf::Vector2f(window->getSize().x / 3.871f + x_c*window->getSize().x / 31.15f, window->getSize().y / 36.f), sf::Vector2i(window->getSize().x / 29.75f, window->getSize().y / 20.145f),"",0);
            m_letters[x_c][y]->setImage(std::string("static/letters/pl/") + word[x_c-x] + ".png");
        }
    }

    return score;

}

