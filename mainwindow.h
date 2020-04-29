#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void movePowerup(int,int,int);
    void moveBall(int,int);
    void moveLeftPaddle(int);
    void moveRightPaddle(int);
    int checkBallWallCollision();
    int checkBallPaddleCollision();
    int checkBallGoalCollision();
    void gameOver(int);
    void matchOver(int);
    void nextGame();
    int checkSelectedMaxScore();
    int checkSelectedGameSpeed();

protected:
    void keyPressEvent(QKeyEvent*);

private slots:
    //void populateTextEdit();
    void spawnPowerup();

private:
    Ui::MainWindow *ui;
    void setUpMenu();
};
#endif // MAINWINDOW_H
