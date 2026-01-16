#ifndef BOMB_H
#define BOMB_H
#include "config.h"
#include <QPixmap>
#include <QVector>

class bomb
{
public:
    bomb();
    //更新坐标
    void updateInfo();
public:
    //数组
    QVector<QPixmap>m_pixArr;
    //坐标
    int m_X;
    int m_Y;
    //空闲
    bool m_Free;
    //记录间隔
    int m_Recorder;
    //图片数标
    int m_index;
};

#endif // BOMB_H
