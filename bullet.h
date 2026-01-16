#ifndef BULLET_H
#define BULLET_H
#include "config.h"
#include <QPixmap>

class Bullet
{
public:
    Bullet();
    //更新坐标
    void updatePosition();
public:
    //duxiang
    QPixmap m_Bullet;
    //坐标
    int m_X;
    int m_Y;
    //speed
    int m_Speed;
    //xinazhi
    bool m_Free;
    //biankuang
    QRect m_Rect;
};

#endif // BULLET_H
