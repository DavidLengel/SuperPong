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
    ui->paddle1->move(ui->paddle1->x(), rand() % 360 + 40);
}
