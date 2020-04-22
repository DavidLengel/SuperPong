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

// Return 1 if top wall, 2 if bottom wall, 0 if false
int MainWindow::checkWallCollision()
{
    if((ui->topWall->y() + ui->topWall->height()) >= ui->ball->y())
    {
        return 1;
    }
    else if(ui->bottomWall->y() <= ui->ball->y() + ui->ball->height())
    {
        return 2;
    }
    else
        return 0;
}

// Return 1 if Paddle 1 (Left), 2 if Paddle 2 (Right), 0 if false
int MainWindow::checkPaddleCollision()
{
    if((ui->paddle1->x() + ui->paddle1->width() >= ui->ball->x())
            && ((ui->ball->y() >= ui->paddle1->y()) && (ui->ball->y() <= ui->paddle1->y() + ui->paddle1->height())))
    {
        return 1;
    }
    else if((ui->paddle2->x() <= ui->ball->x() + ui->ball->width())
            && ((ui->ball->y() >= ui->paddle2->y()) && (ui->ball->y() <= ui->paddle2->y() + ui->paddle2->height())))
    {
        return 2;
    }
    else
        return 0;
}

// Return 1 if Goal 1 (Left), 2 if Goal 2 (Right), 0 if false
int MainWindow::checkGoalCollision()
{
    if(ui->goal1->x() >= ui->ball->x())
    {
        return 1;
    }
    else if (ui->goal2->x() <= ui->ball->x() + ui->ball->width())
    {
        return 2;
    }
    else
        return 0;
}

void MainWindow::gameOver(int winner)
{
    if (winner == 1)
        ui->p1Score->setText(QString::number((ui->p1Score->text().toInt()) + 1));
    else
        ui->p2Score->setText(QString::number((ui->p2Score->text().toInt()) + 1));
}
