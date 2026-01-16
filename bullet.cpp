#include "bullet.h"



Bullet::Bullet()
{
  //jiazai
    m_Bullet.load(BULLET_PATH);
  //坐标
    m_X= GAME_WIDTH*0.5 - m_Bullet.width()*0.5;
    m_Y= GAME_HEIGHT;
    //默认空闲
    m_Free=true;
    //speed
    m_Speed = BULLET_SPEED;
    //边框
    m_Rect.setWidth(m_Bullet.width());
    m_Rect.setHeight(m_Bullet.height());
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::updatePosition()
{
    if(m_Free)
    {
        return;
    }
    //上移
    m_Y -=m_Speed;
    m_Rect.moveTo(m_X,m_Y);

    if(m_Y<= -m_Rect.height())
    {
        m_Free = true;
    }
}
