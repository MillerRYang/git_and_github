// heroplane.cpp
#include "heroplane.h"
#include "config.h"
#include <QDebug>

HeroPlane::HeroPlane()
{

    m_Plane.load(HERO_PATH);

    // 坐标
    m_X = (GAME_WIDTH - m_Plane.width()) * 0.5;
    m_Y = GAME_HEIGHT - m_Plane.height();

    // 边框
    m_Rect.setWidth(m_Plane.width());
    m_Rect.setHeight(m_Plane.height());
    m_Rect.moveTo(m_X, m_Y);

    m_recorder = 0;

    // 初始化生命值和状态
    m_life = 3;
    m_shieldActive = false;
    m_shieldTime = 0;
}

void HeroPlane::shoot()
{

    m_recorder++;

    if (m_recorder < BULLET_INTERVAL) {
        return;
    }


    m_recorder = 0;


    for (int i = 0; i < BULLET_NUM; i++) {
        if (m_bullets[i].m_Free) {
            m_bullets[i].m_Free = false;
            // 坐标
            m_bullets[i].m_X = m_X + m_Rect.width() * 0.25 - 20;
            m_bullets[i].m_Y = m_Y - 50;
            break;
        }
    }
}

void HeroPlane::setPosition(int x, int y)
{
    m_X = x;
    m_Y = y;
    m_Rect.moveTo(m_X, m_Y);
}

void HeroPlane::activateShield()
{
    m_shieldActive = true;
    m_shieldTime = PROP_DURATION;
}

void HeroPlane::increaseLife()
{
    m_life++;
    if (m_life > 5) m_life = 5;
}

void HeroPlane::takeDamage()
{
    if (m_shieldActive) {
        m_shieldActive = false;
        m_shieldTime = 0;
    } else {
        m_life--;
    }
}

void HeroPlane::updateShield()
{
    if (m_shieldActive) {
        m_shieldTime -= GAME_RATE;
        if (m_shieldTime <= 0) {
            m_shieldActive = false;
        }
    }
}
