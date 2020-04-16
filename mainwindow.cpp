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
    ui->paddle1->move(ui->paddle1->x(), rand() % (ui->gameField->height() - ui->paddle1->height()) + ui->gameField->y());
}

void MainWindow::spawnPowerup()
{
    QLabel *powerup = new QLabel("powerup");
    powerup->setFrameStyle(QFrame::Plain);
    powerup->setText("Testaroonie");
    powerup->move(200, 200);
    ui->horizontalLayout->addWidget(powerup);
//    switch(/*rand() % 3*/0)
//    {
//    case 0:
//        break;
//    case 1:
//        break;
//    case 2:
//        break;
//    default:
//        printf("ERROR: Spawn Powerup Failed\n");
//    }
}
