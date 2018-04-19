#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameView.h"
#include "GameField.h"
#include <QHBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    GameField* field;           //Модель игрового поле
    GameView* game;             //Экземпляр игрового процесса
    bool isAI;                  //Состояние ИИ


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;         //Класс интерфейса главного окна


public slots:
    void newGame();                                                     //Начать игру сначала
    void resetGame();                                                   //Сброс игры и настроек на стандартные
    void setup();                                                       //Отображение настроек игры
    void resume();                                                      //Возврат из настроек без сохранения
    void writeWinner(const QString& _str, GameView::WinType _winner);   //Отображение победителя, остановка игры
    void viewMode(bool _isFull);                                        //Переключение полноэкранного режима
    void AIMode(bool _AI);
    void about();                                                       //Отображение информации "о программе"
};

#endif // MAINWINDOW_H
