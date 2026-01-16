// propfactory.cpp
#include "propfactory.h"
#include "config.h"

Prop* PropFactory::createProp(int type, int x, int y)
{
    Prop* prop = new Prop();
    prop->m_type = type;
    prop->m_X = x;
    prop->m_Y = y;
    prop->m_free = false;
    prop->m_rect.moveTo(x, y);

    switch (type) {
    case SHIELD:
        prop->m_pixmap.load(SHIELD_PATH);
        break;
    case HEART:
        prop->m_pixmap.load(HEART_PATH);
        break;
    }

    return prop;
}
