#include "bomb.h"

bomb::bomb()
{
    for(int i=0;i<=BOMB_MAX;i++)
    {
        QString str = QString(BOMB_PATH).arg(i);
        m_pixArr.push_back(QPixmap(str));
    }
    //坐标
    m_X=0;
    m_Y=0;
    m_Free=true;
    m_index =0;
    m_Recorder = 0;
}

void bomb::updateInfo()
{
//空闲状态爆炸
    if(m_Free){return;}
    m_Recorder++;

    if(m_Recorder<BOMB_INTERVAL){return;}
    m_Recorder=0;
    m_index++;
    if(m_index>BOMB_MAX-1)
    {
        m_index = 0;
        m_Free = true;
    }
}
