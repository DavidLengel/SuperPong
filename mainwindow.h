#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define SIZE_ADJUSTMENT 50

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void movePowerup(int,int);
    void moveBall(int,int);
    void moveLeftPaddle(int);
    void moveRightPaddle(int);
    int checkBallWallCollision();
    int checkBallPaddleCollision();
    int checkBallGoalCollision();
    int checkPowerupWallCollision();
    int checkPowerupPaddleCollision();
    int checkPowerupGoalCollision();
    void gameOver(int);
    void matchOver(int);
    void nextGame();
    int checkSelectedMaxScore();
    void spawnPowerup(int);
    void despawnPowerup();
    void activatePowerup(int,int);
    void deactivatePowerup(int);
    void centerPowerup();
    void setPowerupLabelOpacity(int,float);

protected:
    void keyPressEvent(QKeyEvent*);

private slots:
    //void populateTextEdit();

private:
    Ui::MainWindow *ui;
    void setUpMenu();
    void setUpPowerupLabels();
};
#endif // MAINWINDOW_H
