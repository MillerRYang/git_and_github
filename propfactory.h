// propfactory.h
#ifndef PROPFACTORY_H
#define PROPFACTORY_H

#include "prop.h"

// 工厂模式：创建道具
class PropFactory
{
public:
    static Prop* createProp(int type, int x, int y);

    // 道具类型
    enum PropType {
        SHIELD = 0,
        HEART = 1
    };
};

#endif // PROPFACTORY_H
