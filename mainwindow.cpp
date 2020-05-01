#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    grabKeyboard();
    setUpMenu();
    setUpPowerupLabels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpMenu()
{
    QActionGroup *MaxScoreGroup = new QActionGroup(ui->menuMax_Score);
    MaxScoreGroup->addAction(ui->action7);
    MaxScoreGroup->addAction(ui->action14);
    MaxScoreGroup->addAction(ui->action21);

    QActionGroup *GameSpeedGroup = new QActionGroup(ui->menuGame_Speed);
    GameSpeedGroup->addAction(ui->actionSlow);
    GameSpeedGroup->addAction(ui->actionNormal);
    GameSpeedGroup->addAction(ui->actionFast);
    GameSpeedGroup->addAction(ui->actionInsanelyFast);
}

void MainWindow::setUpPowerupLabels()
{
    setPowerupLabelOpacity(1, 0.25);
    QPixmap pixmapTarget = QPixmap("/home/images/ChargePowerup.png");
    ui->powerupChargeLabel->setPixmap(pixmapTarget);

    setPowerupLabelOpacity(2, 0.25);
    pixmapTarget = QPixmap("/home/images/ExtendPowerup.png");
    ui->powerupExtendLabel->setPixmap(pixmapTarget);

    setPowerupLabelOpacity(3, 0.25);
    pixmapTarget = QPixmap("/home/images/ShrinkPowerup.png");
    ui->powerupShrinkLabel->setPixmap(pixmapTarget);
}

void MainWindow::setPowerupLabelOpacity(int powerup, float opacity)
{
    QGraphicsOpacityEffect *op = new QGraphicsOpacityEffect();
    op->setOpacity(opacity);

    switch(powerup)
    {
    case 1:
        ui->powerupChargeLabel->setGraphicsEffect(op);
        break;
    case 2:
        ui->powerupExtendLabel->setGraphicsEffect(op);
        break;
    case 3:
        ui->powerupShrinkLabel->setGraphicsEffect(op);
        break;
    }
}

int MainWindow::checkSelectedMaxScore()
{
    if(ui->action7->isChecked())
        return 7;
    else if(ui->action14->isChecked())
        return 14;
    else if(ui->action21->isChecked())
        return 21;
    else
        return -1;
}

void MainWindow::spawnPowerup(int powerup)
{
    QPixmap pixmapTarget;

    switch(powerup)
    {
    case 1:
        pixmapTarget = QPixmap("/home/images/ChargePowerup.png");
        break;
    case 2:
        pixmapTarget = QPixmap("/home/images/ExtendPowerup.png");
        break;
    case 3:
        pixmapTarget = QPixmap("/home/images/ShrinkPowerup.png");
        break;
    default:
        perror("Powerup number invalid!");
    }
    ui->powerup->setPixmap(pixmapTarget);
    ui->powerup->setVisible(true);
}

void MainWindow::despawnPowerup()
{
    ui->powerup->setVisible(false);
}

void MainWindow::activatePowerup(int powerup, int paddle)
{
    despawnPowerup();
    setPowerupLabelOpacity(powerup, 1);
    switch(powerup)
    {
    case 1:
        break;
    case 2:
        if (paddle == 1)
        {
            ui->paddle1->resize(ui->paddle1->width(), ui->paddle1->height() + SIZE_ADJUSTMENT);
        }
        else
        {
            ui->paddle2->resize(ui->paddle2->width(), ui->paddle2->height() + SIZE_ADJUSTMENT);
        }
        break;
    case 3:
        if (paddle == 1)
        {
            ui->paddle2->resize(ui->paddle2->width(), ui->paddle2->height() - SIZE_ADJUSTMENT);
        }
        else
        {
            ui->paddle1->resize(ui->paddle1->width(), ui->paddle1->height() - SIZE_ADJUSTMENT);
        }
        break;
    default:
        perror("Invalid powerup id passed to activatePowerup()!");
    }
}

void MainWindow::deactivatePowerup(int powerup)
{
    const int original_paddle_size = 100;
    setPowerupLabelOpacity(powerup, 0.25);

    if (powerup == 2 || powerup == 3)
    {
        ui->paddle1->resize(ui->paddle1->width(), original_paddle_size);
        ui->paddle2->resize(ui->paddle2->width(), original_paddle_size);
    }
}

void MainWindow::movePowerup(int x_coord, int y_coord)
{
    ui->powerup->move(ui->gameField->x()+x_coord, ui->gameField->y()+y_coord);
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
int MainWindow::checkBallWallCollision()
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
int MainWindow::checkBallPaddleCollision()
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
int MainWindow::checkBallGoalCollision()
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

int MainWindow::checkPowerupGoalCollision()
{
    if(ui->goal1->x() >= ui->powerup->x())
    {
        return 1;
    }
    else if (ui->goal2->x() <= ui->powerup->x() + ui->powerup->width())
    {
        return 2;
    }
    else
        return 0;
}

int MainWindow::checkPowerupWallCollision()
{
    if((ui->topWall->y() + ui->topWall->height()) >= ui->powerup->y())
    {
        return 1;
    }
    else if(ui->bottomWall->y() <= ui->powerup->y() + ui->powerup->height())
    {
        return 2;
    }
    else
        return 0;
}

int MainWindow::checkPowerupPaddleCollision()
{
    if((ui->paddle1->x() + ui->paddle1->width() >= ui->powerup->x())
            && ((ui->powerup->y() >= ui->paddle1->y()) && (ui->powerup->y() <= ui->paddle1->y() + ui->paddle1->height())))
    {
        return 1;
    }
    else if((ui->paddle2->x() <= ui->powerup->x() + ui->powerup->width())
            && ((ui->powerup->y() >= ui->paddle2->y()) && (ui->powerup->y() <= ui->paddle2->y() + ui->paddle2->height())))
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

    setPowerupLabelOpacity(1, 0.25);
    setPowerupLabelOpacity(2, 0.25);
    setPowerupLabelOpacity(3, 0.25);
}

void MainWindow::matchOver(int winner)
{
    ui->winnerLabel->setText("Player " + QString::number(winner) + " won!");
}

void MainWindow::nextGame()
{
    ui->gameCount->setText(QString::number(ui->gameCount->text().toInt() + 1));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_W:
            ui->paddle1->move(ui->paddle1->x(), ui->paddle1->y()-std::min(30,(ui->paddle1->y()-ui->topWall->y()-ui->topWall->height())));
            break;
        case Qt::Key_S:
            ui->paddle1->move(ui->paddle1->x(), ui->paddle1->y()+std::min(30,(ui->bottomWall->y()-ui->paddle1->y()-ui->paddle1->height())));
            break;
        case Qt::Key_Up:
            ui->paddle2->move(ui->paddle2->x(), ui->paddle2->y()-std::min(30,(ui->paddle2->y()-ui->topWall->y()-ui->topWall->height())));
            break;
        case Qt::Key_Down:
            ui->paddle2->move(ui->paddle2->x(), ui->paddle2->y()+std::min(30,(ui->bottomWall->y()-ui->paddle2->y()-ui->paddle2->height())));
            break;
//        case Qt::Key_Space:

    }
}

void MainWindow::centerPowerup()
{
    ui->powerup->move(ui->gameField->x() + ui->gameField->width()/2 - ui->powerup->width()/2, ui->gameField->y() + ui->gameField->height()/2 - ui->powerup->height()/2);
}
