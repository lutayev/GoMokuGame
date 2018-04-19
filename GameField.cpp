#include "GameField.h"

GameField::GameField(uint _sizeX, uint _sizeY)
: mySizeX(_sizeX), mySizeY(_sizeY)
{
    myField = new char*[mySizeX];

    for (uint x = 0; x < mySizeX; ++x)
    {
        myField[x] = new char[mySizeY + 1];
        for (uint y = 0; y < mySizeY; ++y)
            myField[x][y] = ' ';
        myField[x][mySizeY] = 0;
    }
}

bool GameField::move(uint _x, uint _y, char _marker)
{
    char tmp = getCell(_x, _y);
    if ((tmp == ' ') && (_x <= mySizeX) && (_y <= mySizeY))
    {
        myField[_x - 1][_y - 1] = _marker;
        return true;
    }
    else return false;
}

char GameField::getCell(uint _x, uint _y) const
{
    if ( (_x <= mySizeX) && (_y <= mySizeY) )
    {
        return myField[_x - 1][_y - 1];
    }
    else return '\0';
}

uint GameField::getSizeX() const
{
    return mySizeX;
}

uint GameField::getSizeY() const
{
    return mySizeY;
}

GameField::~GameField()
{
    for (uint x = 0; x < mySizeX; x++)
        delete [] myField[x];
    delete [] myField;
}
