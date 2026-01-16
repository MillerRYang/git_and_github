#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include "map.h"
#include "heroplane.h"
#include "enermyplane.h"
#include "bomb.h"
#include "prop.h"
#include "gamestate.h"

class MainScene : public QWidget
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();


    void initScene();
    void playGame();
    void updatePosition();

    void paintEvent(QPaintEvent *)override;


    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void startNewGame(int difficulty);

    void drawUI(QPainter* painter);


    void drawStartMenu(QPainter* painter);


    void drawGameOver(QPainter* painter);


    bool isInGame() const { return m_isInGame; }

private slots:
    void updateEnemySpeed();  // 动态难度

private:
    // 地图
    Map m_map;

    // 飞机
    HeroPlane m_hero;

    // 定时器
    QTimer m_Timer;
    QTimer m_difficultyTimer;

    // 敌机
    void enermyToScene();
    enermyPlane m_enermys[ENEMY_NUM];
    int m_recorder;


    Prop* m_props[PROP_NUM];
    int m_propRecorder;
    void generateProp();
    void updateProps();
    void checkPropCollision();


    void collisionDetection();


    bomb m_bombs[BOMB_NUM];


    GameState* m_gameState;


    int m_enemySpeedIncrement;


    bool m_isInGame;
    bool m_showStartMenu;
    bool m_showGameOver;


    QRect m_easyButtonRect;
    QRect m_hardButtonRect;
    QRect m_restartButtonRect;
};

#endif // MAINSCENE_H
