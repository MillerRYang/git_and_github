// prop.cpp
#include "prop.h"
#include "config.h"

Prop::Prop()
{
    m_free = true;
    m_speed = 3;
    m_rect.setWidth(40);
    m_rect.setHeight(40);
}

void Prop::updatePosition()
{
    if (m_free) return;

    m_Y += m_speed;
    m_rect.moveTo(m_X, m_Y);

    // 超出屏幕则重置
    if (m_Y > GAME_HEIGHT) {
        m_free = true;
    }
}

QRect Prop::getRect()
{
    return m_rect;
}

void Prop::reset()
{
    m_free = true;
}
