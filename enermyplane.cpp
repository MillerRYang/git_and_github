#include "enermyplane.h"
#include "config.h"

enermyPlane::enermyPlane()
{//初始化
    m_enermy.load(ENEMY_PATH);
    m_X =0;
    m_Y= 0;
//空闲
    m_Free = true;
//速度
    m_Speed = ENEMY_SPEED;
//边框
    m_Rect.setWidth(m_enermy.width());
    m_Rect.setHeight(m_enermy.height());
    m_Rect.moveTo(m_X,m_Y);
}

void enermyPlane::updatePosition()
{
   //空闲不计算坐标
    if(m_Free)
   {
       return;
    }

    m_Y += m_Speed;
    m_Rect.moveTo(m_X,m_Y);
    //chongzhi
    if(m_Y>=GAME_HEIGHT + m_Rect.height())
    {
        m_Free=true;
    }
}
