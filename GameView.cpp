#include "GameView.h"
#include <QPixmap>
#include <QSpacerItem>
#include "QResource"
#include <cstdlib>
#include <ctime>

GameView::GameView(GameField* _field, uint _line) :
    QWidget(0), mainField(_field)
{
    allView = new QVBoxLayout;
    fieldView = new QGridLayout;
    horisontalLay = new QHBoxLayout;
    info1 = new QLabel;
    info2 = new QLabel;

    p1 = new Player;
    p2 = new Player;
    p1->name = "X";
    p1->marker = 'X';
    p1->nextPlayer = p2;            //p1 указывает на p2
    p1->AI = false;
    p2->name = "O";
    p2->marker = 'O';
    p2->AI = false;
    p2->nextPlayer = p1;            //p2 указывает на p1
    currPlayer = p1;                //Первым ходит p1

    buttonCross = new QIcon(QPixmap(":/Resource/cross.png"));
    buttonCircle = new QIcon(QPixmap(":/Resource/circle.png"));
    buttonEmpty = new QIcon(QPixmap(":/Resource/empty.png"));

    uint sizeX = mainField->getSizeX();
    uint sizeY = mainField->getSizeY();
    uint buttonSize = 400 / (sizeX);
    if(sizeX > _line) countToWin = _line;
    else countToWin = sizeX;

    QPushButton* pcmd;              //Указатель на создаваемые ячейки поля (кнопки)
    buttonsArray = new QPushButton**[sizeX];    //Массив строк указателей на ячейки
    for(uint i = 0; i < sizeY; i++)
    {
        buttonsArray[i] = new QPushButton*[sizeY];  //Столбцы указателей на ячейки
        for(uint j = 0; j < sizeY; j++)
        {
            pcmd = new QPushButton("");             //Создание, инициализация и настройка ячейки поля
            pcmd->setFlat(true);
            pcmd->setIcon(*buttonEmpty);
            pcmd->setIconSize(QSize(buttonSize, buttonSize));
            pcmd->setMinimumSize(buttonSize, buttonSize);
            pcmd->setMaximumSize(buttonSize, buttonSize);
            connect(pcmd, SIGNAL(clicked()), this, SLOT(mainLoop()));    //Соединение ячейки с игровым циклом
            buttonsArray[i][j] = pcmd;                                   //Добавление указателя на ячейку в массив
            fieldView->addWidget(pcmd, i, j);                            //Добавление ячейки на слой отображения поля
        }
    }

    horisontalLay->addItem(fieldView);                                  //Добавление поля на вспомагательный слой
    allView->addItem(horisontalLay);                                    //Добавление вспомагательного слоя на основной слой
    allView->addWidget(info1);                                           //Добавление информации о ходе на основной слой
    allView->addWidget(info2);
    setLayout(allView);                                                 //Установка главного слоя

    connect(this, SIGNAL(moved()), this, SLOT(checkWinner()));          //Соединение сигнала хода со слотом проверки победителя
    info1->setText("Сейчас ходит: " + currPlayer->name + ".");
    info2->setText("Для победы нужно " + QString::number(countToWin) + " в ряд.");               //Установка информации о текущем ходе
}

bool GameView::makeMove(Player* _p)                                     //Попытка осуществить ход с проверкой корректности
{
    if (mainField->move(_p->x, _p->y, _p->marker))
        return true;
    else return false;
}

void GameView::AIMode(bool _AI)
{
    p2->AI = _AI;
    if (currPlayer == p2)
    {
        generateAI(currPlayer);
        emit buttonsArray[(currPlayer->x - 1)][(currPlayer->y - 1)]->clicked();
        return;
    }
}

void GameView::generateAI(GameView::Player *_p)
{
    srand(time(0));
    while(true)
    {
        uint x = (rand() % mainField->getSizeX()) + 1;
        uint y = (rand() % mainField->getSizeY()) + 1;
        if(mainField->getCell(x, y) == ' ')
        {
            _p->x = x;
            _p->y = y;
            return;
        }
    }
}

void GameView::mainLoop()            //главный игровой цикл
{
    bool correctMove = false;

    QPushButton* pSender = qobject_cast<QPushButton*>(QObject::sender()); //берем указатель нажатой кнопки
    for(uint i = 0; i < mainField->getSizeX(); ++i)
        for(uint j = 0; j < mainField->getSizeY(); ++j)
        {
            if(pSender == buttonsArray[i][j]) //находим кнопку в массиве, пытаемся сделать соответствующий ход
            {
                currPlayer->x = i + 1;
                currPlayer->y = j + 1;
                correctMove = makeMove(currPlayer);
            }

            if(correctMove)                 //если ход корректный меняем вид кнопки, меняем текущего игрока
            {
                if(currPlayer == p1)
                    buttonsArray[i][j]->setIcon(*buttonCross);
                else buttonsArray[i][j]->setIcon(*buttonCircle);
                currPlayer = currPlayer->nextPlayer;
                info1->setText("Сейчас ходит: " + currPlayer->name);
                //info2->setText("Для победы нужно " + QString::number(countToWin) + " в ряд.");
                emit moved();

                if ((currPlayer->AI == true) && (whoWin() == PLAY)) //если следующий игрок ИИ генерируем координаты и посылаем сигнал
                {
                    generateAI(currPlayer);
                    emit buttonsArray[(currPlayer->x - 1)][(currPlayer->y - 1)]->clicked();
                    return;
                }
                return;
            }
            else
            {
                info1->setText("Неправильный ход!!!! Сейчас ходит: " + currPlayer->name);
            }
        }
}

GameView::WinType GameView::whoWin()                            //Логика проверки поля на выигрыш
{
    uint p1Count = 0;
    uint p2Count = 0;
    bool emptyCount = false;
    char temp;
    char prev = ' ';

    for (uint i = 1; i <= mainField->getSizeX(); i++)			//Проверка строк
    {
        for (uint j = 1; j <= mainField->getSizeY(); j++)
        {
            temp = mainField->getCell(i, j);								//Перебор и проверка здесь
            if (temp == p1->marker && temp == prev)	p1Count++;
            else p1Count = 0;
            if (temp == p2->marker && temp == prev)	p2Count++;
            else p2Count = 0;
            if (temp == ' ') emptyCount = true;
            prev = temp;
            if (p1Count >= countToWin - 1) return XWIN;
            if (p2Count >= countToWin - 1) return YWIN;
        }
        p1Count = 0;
        p2Count = 0;
        prev = ' ';
    }

    for (uint i = 1; i <= mainField->getSizeY(); i++)			//Проверка столбцов
    {
        for (uint j = 1; j <= mainField->getSizeX(); j++)
        {
            temp = mainField->getCell(j, i);								//Перебор и проверка здесь
            if (temp == p1->marker && temp == prev)	p1Count++;
            else p1Count = 0;
            if (temp == p2->marker && temp == prev)	p2Count++;
            else p2Count = 0;
            if (temp == ' ') emptyCount = true;
            prev = temp;
            if (p1Count >= countToWin - 1) return XWIN;
            if (p2Count >= countToWin - 1) return YWIN;
        }
        p1Count = 0;
        p2Count = 0;
        prev = ' ';
    }

    for (uint i = 1; i <= mainField->getSizeX() - 2; i++)     //Проверка прямых диагоналей ниже первой диагонали "\"
    {																				//По иксам бегаем от 1 до (max-2)
        for (uint k = 0; k <= mainField->getSizeX() - i; k++)						//По игрекам бегаем
        {
            temp = mainField->getCell(i + k, k + 1);								//Перебор и проверка здесь
            if (temp == p1->marker && temp == prev)	p1Count++;
            else p1Count = 0;
            if (temp == p2->marker && temp == prev)	p2Count++;
            else p2Count = 0;
            if (temp == ' ') emptyCount = true;
            prev = temp;
            if (p1Count >= countToWin - 1) return XWIN;
            if (p2Count >= countToWin - 1) return YWIN;
        }
        p1Count = 0;
        p2Count = 0;
        prev = ' ';
    }

    for (uint i = 1; i <= mainField->getSizeY() - 2; i++)		//Проверка прямых диагоналей выше первой диагонали "\"
    {
        for (uint k = 0; k <= mainField->getSizeY() - i; k++)
        {
            temp = mainField->getCell(k + 1, i + k);								//Перебор и проверка здесь
            if (temp == p1->marker && temp == prev)	p1Count++;
            else p1Count = 0;
            if (temp == p2->marker && temp == prev)	p2Count++;
            else p2Count = 0;
            if (temp == ' ') emptyCount = true;
            prev = temp;
            if (p1Count >= countToWin - 1) return XWIN;
            if (p2Count >= countToWin - 1) return YWIN;
        }
        p1Count = 0;
        p2Count = 0;
        prev = ' ';
    }

    for (uint i = 1; i <= mainField->getSizeX() - 2; i++)		//Проверка прямых диагоналей ниже второй диагонали "/"
    {																				//По иксам снова бегаем от 1 до (max-2)
        for (uint k = 0; k <= mainField->getSizeX() - i; k++)						//По игрекам бегаем
        {
            temp = mainField->getCell(k + 1, mainField->getSizeY() - (k + i - 1));								//Перебор и проверка здесь
            if (temp == p1->marker && temp == prev)	p1Count++;
            else p1Count = 0;
            if (temp == p2->marker && temp == prev)	p2Count++;
            else p2Count = 0;
            if (temp == ' ') emptyCount = true;
            prev = temp;
            if (p1Count >= countToWin - 1) return XWIN;
            if (p2Count >= countToWin - 1) return YWIN;
        }
        p1Count = 0;
        p2Count = 0;
        prev = ' ';
    }

    for (uint i = 1; i <= mainField->getSizeX() - 2; i++)			//Проверка прямых диагоналей выше второй диагонали "/"
    {																				//По иксам бегаем от 1 до (max-2)
        for (uint k = 0; k <= mainField->getSizeX() - i; k++)						//По игрекам бегаем
        {
            temp = mainField->getCell(i + k, mainField->getSizeY() - k);								//Перебор и проверка здесь
            if (temp == p1->marker && temp == prev)	p1Count++;
            else p1Count = 0;
            if (temp == p2->marker && temp == prev)	p2Count++;
            else p2Count = 0;
            if (temp == ' ') emptyCount = true;
            prev = temp;
            if (p1Count >= countToWin - 1) return XWIN;
            if (p2Count >= countToWin - 1) return YWIN;
        }
        p1Count = 0;
        p2Count = 0;
        prev = ' ';
    }

    if (!emptyCount) return NOWIN;                                  //Не осталось пустых мест
    return PLAY;                                                    //Продолжаем играть
}

void GameView::checkWinner()                                        //Слот проверки победителя, издает сигнал с победителем
{
    WinType winner = whoWin();
    switch (winner)
    {
    case XWIN: emit win("победил!!!", winner); break;

    case YWIN: emit win("победил!!!", winner); break;

    case NOWIN: emit win("Ничья!!!", winner); break;

    case PLAY: break;
    }
}

GameView::~GameView()                                           //Освобождаем ресурсы
{
    delete p1;
    delete p2;
    for (uint i = 0; i < mainField->getSizeX(); ++i)
    {
        for(uint j = 0; j < mainField->getSizeY(); ++j)
            delete buttonsArray[i][j];
        delete [] buttonsArray[i];
    }
    delete [] buttonsArray;
}
