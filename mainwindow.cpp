#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSpinBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isAI = false;

    field = new GameField(5, 5);                //Создаем модель поля
    game = new GameView(field, 4);                 //Создаем экземпляр игры
    ui->label->hide();                          //Скрываем ненужные виджеты главного окна
    ui->label_2->hide();
    ui->horizontalLayout->addWidget(game);
    ui->settingsWidget->hide();                 //"Прячем" слой настроек
    connect(ui->action_reset, SIGNAL(triggered()), this, SLOT(resetGame()));
    connect(ui->action_setup, SIGNAL(triggered()), this, SLOT(setup()));
    connect(ui->resumeGame, SIGNAL(clicked()), this, SLOT(resume()));
    connect(ui->action_fullscreen, SIGNAL(triggered(bool)), this, SLOT(viewMode(bool)));
    connect(ui->action_AI, SIGNAL(triggered(bool)), this, SLOT(AIMode(bool)));
    connect(ui->action_about, SIGNAL(triggered()), this, SLOT(about()));
    connect(game, SIGNAL(win(QString, GameView::WinType)), this, SLOT(writeWinner(QString, GameView::WinType)));
}

void MainWindow::newGame()
{
    ui->label->setText("");
    ui->label->hide();
    ui->label_2->hide();
    ui->horizontalLayout->removeWidget(game);

    delete game;                                //Освобождаем ресурсы
    delete field;

    int i = ui->spinBoxField->text().toInt();   //Считываем размер поля и длину линии для победы из настроек
    int line = ui->spinBoxLine->text().toInt();
    field = new GameField(i, i);                //Создаем новое поле и игру
    game = new GameView(field, line);           //Устанавливаем поле и размер линии для победы
    game->p2->AI = isAI;                        //Устанавливаем режим ИИ из настроек

    ui->horizontalLayout->addWidget(game);
    ui->settingsWidget->hide();
    connect(ui->action_reset, SIGNAL(triggered()), this, SLOT(resetGame()));
    connect(ui->action_setup, SIGNAL(triggered()), this, SLOT(setup()));
    connect(ui->resumeGame, SIGNAL(clicked()), this, SLOT(resume()));
    connect(ui->action_AI, SIGNAL(triggered(bool)), this, SLOT(AIMode(bool)));
    connect(game, SIGNAL(win(QString, GameView::WinType)), this, SLOT(writeWinner(QString, GameView::WinType)));
}

void MainWindow::resetGame()
{
    ui->spinBoxField->setValue(5);           //Устанавливаем размер поля 5х5 "по умолчанию"
    ui->spinBoxLine->setValue(4);
    newGame();
}

void MainWindow::setup()
{
    game->hide();                       //Прячем слой игрового поля, показываем слой настроек
    ui->settingsWidget->show();
}

void MainWindow::resume()
{
    ui->settingsWidget->hide();         //Прячем слой настроек, показываем слой игрового поля
    game->show();
}

void MainWindow::writeWinner(const QString& _str, GameView::WinType _winner)
{
    GameView::WinType winner = _winner;
    switch (winner)
    {
        case GameView::XWIN: ui->label_2->setPixmap(QPixmap(":/Resource/cross_small.png")); //Показ иконки победителя
        ui->label->setText(_str);
        ui->label->show();
        ui->label_2->show();
        break;

        case GameView::YWIN: ui->label_2->setPixmap(QPixmap(":/Resource/circle_small.png"));
        ui->label->setText(_str);
        ui->label->show();
        ui->label_2->show();
        break;

        case GameView::NOWIN: ui->label_2->setPixmap(QPixmap(":/Resource/empty_small.png"));
        ui->label->setText(_str);
        ui->label->show();
        ui->label_2->show();
        break;
    }
    game->setDisabled(true);                //Деактивируем игровое поле
}

void MainWindow::viewMode(bool _isFull)     //Переключение полноэкранного режима
{
    if(_isFull)
        this->showFullScreen();
    else this->showNormal();
}

void MainWindow::AIMode(bool _AI)
{
    isAI = _AI;
    game->AIMode(_AI);
}

void MainWindow::about()                    //О программе
{
    QMessageBox::about(this, tr("О программе"),
                       tr("Программа крестики-нолики, версия 1.0"
                        "<p>Автор: Лутаев Иван, 2018г."
                        "<p><b>E-mail:     i.lutayev@gmail.com</b>"));
}

MainWindow::~MainWindow()
{
    delete game;
    delete field;
    delete ui;
}
