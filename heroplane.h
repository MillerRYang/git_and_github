// heroplane.h
#ifndef HEROPLANE_H
#define HEROPLANE_H
#include<QPixmap>
#include<QRect>
#include "bullet.h"

class HeroPlane
{
public:
    HeroPlane();

    // 发射
    void shoot();

    // 位置
    void setPosition(int x,int y);

    // 护盾
    void activateShield();

    // 增加生命值
    void increaseLife();

    // 受伤
    void takeDamage();

    // 检查
    bool hasShield() const { return m_shieldActive; }

    // 护盾剩余时间
    int getShieldTime() const { return m_shieldTime; }

    // 更新
    void updateShield();

    // 生命值
    int getLife() const { return m_life; }

    // 存活
    bool isAlive() const { return m_life > 0; }

public:
    // 飞机资源
    QPixmap m_Plane;

    // 坐标
    int m_X;
    int m_Y;

    int m_recorder;
    // 边框
    QRect m_Rect;
    // 弹夹
    Bullet m_bullets[BULLET_NUM];

private:
    int m_life;          // 生命值
    bool m_shieldActive; // 是否激活
    int m_shieldTime;    // 护盾剩余
};

#endif // HEROPLANE_H
