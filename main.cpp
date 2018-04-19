#include <QApplication>
#include <QWidget>
#include "GameField.h"
#include "GameView.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //GameField field(5, 5);
    //GameView game(&field);
    MainWindow game(NULL);
    game.show();

    return a.exec();
}
