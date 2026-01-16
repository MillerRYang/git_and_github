// prop.h
#ifndef PROP_H
#define PROP_H

#include <QPixmap>
#include <QRect>
#include <QTimer>

class Prop
{
public:
    Prop();


    int m_type;

    // 更新位置
    void updatePosition();

    // 获取道具矩形
    QRect getRect();

    // 重置道具
    void reset();

public:
    QPixmap m_pixmap;
    int m_X;
    int m_Y;
    int m_speed;
    bool m_free;
    QRect m_rect;
};

#endif // PROP_H
