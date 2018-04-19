#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QLayout>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <QIcon>
#include "GameField.h"

class GameView : public QWidget
{
    Q_OBJECT
    struct Player
    {
        uint x;                   //Номер строки для хода
        uint y;                   //Номер столбца для хода
        QString name;               //Имя игрока
        char marker;                //Маркер игрока
        bool AI;                    //ИИ или обычный игрок
        Player* nextPlayer;         //Указатель на следующего игрока
    };

    GameField* mainField;           //Указатель на игровое поле
    uint countToWin;                //Количество взятых ячеек для победы
    QGridLayout* fieldView;         //Слой отображения ячеек поля игры
    QVBoxLayout* allView;           //Слой отображения поля и информации о ходе
    QHBoxLayout* horisontalLay;     //Вспомагательный слой
    QPushButton*** buttonsArray;    //Двумерный динамический массив указателей на кнопки(соответствуют ячейкам игрового поля)
    QIcon* buttonCross;             //Иконка "крестика"
    QIcon* buttonCircle;            //Иконка "нолика"
    QIcon* buttonEmpty;             //Иконка пустой ячейки поля

public:
    Player* p1;                     //Игрок 1
    Player* p2;                     //Игрок 2
    Player* currPlayer;             //Текущий игрок
    void AIMode(bool);                          //Переключение режима ИИ
    explicit GameView(GameField* _field, uint _line = 3);       //Конструктор инициализируется указателем на модель игрового поля
    ~GameView();                                //Деструктор

    enum WinType { XWIN, YWIN, NOWIN, PLAY };   //Перечислимый тип для проверки победителя
    QLabel* info1;                               //Виджет отображения текущей информации о ходе
    QLabel* info2;

    bool makeMove(Player* _p);                  //Метод "ход"
    void generateAI(Player* _p);                //Генерация координат хода для AI
    WinType whoWin();                           //Метод проверки на победителя

signals:
    void moved();                               //Сигнал "сделан ход"
    void win(const QString& _str, GameView::WinType _winner);   //Сигнал об окончании игры, передающий победителя

private:

public slots:
    void mainLoop();                             //Слот, соединенный с ячейками поля. Главный игровой цикл.
    void checkWinner();                          //Надо перенести функционал в метод whoWin(), а вызов в onClick()
};

#endif // GAMEVIEW_H
