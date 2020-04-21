#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateTextEdit()
{
    ui->historyList->addItem("Cool Dude 66");
    //paddle moves between top wall and bottom wall
    ui->paddle1->move(ui->paddle1->x(), rand() % (ui->gameField->height() - ui->paddle1->height()) + ui->gameField->y());
}

void MainWindow::spawnPowerup()
{
    QLabel *powerup = new QLabel("powerup");
    powerup->setFrameStyle(QFrame::Plain);
    powerup->setText("Testaroonie");
    powerup->move(200, 200);
    ui->horizontalLayout->addWidget(powerup);
    switch(rand() % 3)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        printf("ERROR: Spawn Powerup Failed\n");
    }
}

void MainWindow::moveBall(int x_coord, int y_coord)
{
    ui->ball->move(ui->gameField->x()+x_coord, ui->gameField->y()+y_coord);
}

void MainWindow::moveLeftPaddle(int y_coord)
{
    ui->paddle1->move(ui->paddle1->x(), ui->gameField->y()+y_coord);
}

void MainWindow::moveRightPaddle(int y_coord)
{
    ui->paddle2->move(ui->paddle2->x(), ui->gameField->y()+y_coord);
}
