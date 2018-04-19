#ifndef GAMEFIELD_H
#define GAMEFIELD_H

typedef unsigned int uint;

class GameField
{
private:
    const uint mySizeX;                   //Количество строк
    const uint mySizeY;                   //Количество столбцов
    char** myField;                         //Двумерный динамический массив символов для хранения состояния поля

public:
    GameField(uint _sizeX, uint _sizeY);        //Конструкрор
    bool move(uint _x, uint _y, char _marker);  //Метод хода с проверкой корректности
    char getCell(uint _x, uint _y) const;       //Прочесть ячейку
    uint getSizeX() const;                        //Узнать количество строк
    uint getSizeY() const;                        //Узнать количество столбцов
    ~GameField();                                   //Деструктор, удаляет поле
};

#endif
