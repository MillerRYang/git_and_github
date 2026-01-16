#ifndef CONFIG_H
#define CONFIG_H
//游戏的配置
#define GAME_WIDTH  480   //宽度
#define GAME_HEIGHT 852   //高度
#define GAME_TITLE "飞机大战 "//标题
#define GAME_ICON ":/res/app.ico"
#define GAME_RATE 10  //毫秒


#define MAP_PATH ":/res/beijin3"
#define MAP_SCROLL_SPEED 2//速度


#define HERO_PATH ":/res/feiji5"


#define BULLET_PATH ":/res/zidan5"
#define BULLET_SPEED 5//速度
#define BULLET_NUM 30//数量
#define BULLET_INTERVAL 25//间隔时间


#define ENEMY_PATH ":/res/diji1"
#define ENEMY_SPEED 5//速度
#define ENEMY_NUM 20//数量
#define ENEMY_INTERVAL 30//间隔时间


#define BOMB_PATH ":/res/b-%1"
#define BOMB_NUM 20//数量
#define BOMB_MAX 6//最大
#define BOMB_INTERVAL 5//间隔时间


#define PROP_INTERVAL 300  //道具生成
#define PROP_NUM 5         //最大道具数
#define PROP_DURATION 15000 //道具持续时间
#define SHIELD_PATH ":/res/shield"
#define HEART_PATH ":/res/heart"


#define EASY 0
#define HARD 1

#endif // CONFIG_H
