#ifndef MAP_H
#define MAP_H
#include<QPixmap>

class Map
{
public:

    Map();
    void mapPosition();

public:
    //地图
    QPixmap m_map1;
    QPixmap m_map2;

    //地图Y
    int m_map1_posY;
    int m_map2_posY;
    //滚动幅度
    int m_scroll_speed;
};

#endif // MAP_H
