// gamestate.h
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <QSettings>

// 单例模式：管理游戏状态和分数
class GameState : public QObject
{
    Q_OBJECT

private:
    explicit GameState(QObject *parent = nullptr);

public:
    // 获取单例实例
    static GameState* instance();

    // 分数操作
    void addScore(int points);
    void resetScore();
    int getScore() const { return m_score; }

    // 击中敌机数
    void addHitCount();
    int getHitCount() const { return m_hitCount; }

    // 难度设置
    void setDifficulty(int difficulty);
    int getDifficulty() const { return m_difficulty; }

    // 游戏状态
    void setGameOver(bool over);
    bool isGameOver() const { return m_gameOver; }

    // 最高分操作
    int getHighScore(int difficulty);
    void saveHighScore();

    // 重置游戏状态
    void reset();

private:
    static GameState* m_instance;

    int m_score;        // 当前分数
    int m_hitCount;     // 击中敌机数
    int m_difficulty;   // 游戏难度
    bool m_gameOver;    // 游戏是否结束

    // 最高分记录
    int m_highScoreEasy;
    int m_highScoreHard;

    QSettings m_settings;  // 用于保存设置
};

#endif // GAMESTATE_H
