// gamestate.cpp
#include "gamestate.h"
#include <QDebug>
#include "config.h"

GameState* GameState::m_instance = nullptr;

GameState::GameState(QObject *parent)
    : QObject(parent)
    , m_score(0)
    , m_hitCount(0)
    , m_difficulty(0)
    , m_gameOver(false)
    , m_settings("PlaneWar", "HighScores")
{
    // 从QSettings加载最高分
    m_highScoreEasy = m_settings.value("highScoreEasy", 0).toInt();
    m_highScoreHard = m_settings.value("highScoreHard", 0).toInt();
}

GameState* GameState::instance()
{
    if (!m_instance) {
        m_instance = new GameState();
    }
    return m_instance;
}

void GameState::addScore(int points)
{
    m_score += points;
}

void GameState::resetScore()
{
    m_score = 0;
    m_hitCount = 0;
}

void GameState::addHitCount()
{
    m_hitCount++;
}

void GameState::setDifficulty(int difficulty)
{
    m_difficulty = difficulty;
}

void GameState::setGameOver(bool over)
{
    m_gameOver = over;
    if (over) {
        saveHighScore();
    }
}

int GameState::getHighScore(int difficulty)
{
    return (difficulty == EASY) ? m_highScoreEasy : m_highScoreHard;
}

void GameState::saveHighScore()
{
    if (m_difficulty == EASY && m_score > m_highScoreEasy) {
        m_highScoreEasy = m_score;
        m_settings.setValue("highScoreEasy", m_highScoreEasy);
    } else if (m_difficulty == HARD && m_score > m_highScoreHard) {
        m_highScoreHard = m_score;
        m_settings.setValue("highScoreHard", m_highScoreHard);
    }
}

void GameState::reset()
{
    m_score = 0;
    m_hitCount = 0;
    m_gameOver = false;
}
