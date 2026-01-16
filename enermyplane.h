#ifndef ENERMYPLANE_H
#define ENERMYPLANE_H
#include <QPixmap>

class enermyPlane
{
public:
    enermyPlane();
    //更新坐标
    void updatePosition();

public:
    QPixmap m_enermy;
    //坐标
    int m_X;
    int m_Y;
    //边框
    QRect m_Rect;
    //空闲
    bool m_Free;
    //速度
    int m_Speed;
};

#endif // ENERMYPLANE_H
