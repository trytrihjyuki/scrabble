#include"board.hpp"

Bonus::Board()
{
    for(int x=0; x<15; x++){
        for(int y=0; y<15; y++){
            m_letters[x][y] = NULL;
            m_letterBonus[x][y] = 1;
            m_wordBonus[x][y] = 1;
        }
    }
}

Board::~Board()
{

}

