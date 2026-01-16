#include "mainscene.h"
#include "config.h"
#include "propfactory.h"
#include <QIcon>
#include <QPainter>
#include <QMouseEvent>
#include <ctime>
#include <QFont>
#include <QBrush>
#include <QLinearGradient>
#include <QDebug>
#include <QMessageBox>

MainScene::MainScene(QWidget *parent)
    : QWidget(parent)
    , m_gameState(GameState::instance())
    , m_isInGame(false)
    , m_showStartMenu(true)
    , m_showGameOver(false)
{

    m_easyButtonRect = QRect(GAME_WIDTH/2 - 100, GAME_HEIGHT/2 - 20, 200, 40);
    m_hardButtonRect = QRect(GAME_WIDTH/2 - 100, GAME_HEIGHT/2 + 40, 200, 40);
    m_restartButtonRect = QRect(GAME_WIDTH/2 - 100, GAME_HEIGHT/2 + 100, 200, 40);
    initScene();
}

MainScene::~MainScene()
{
    // 清理道具
    for (int i = 0; i < PROP_NUM; i++) {
        if (m_props[i]) {
            delete m_props[i];
        }
    }
}

void MainScene::initScene()
{
    setFixedSize(GAME_WIDTH, GAME_HEIGHT);
    setWindowTitle(GAME_TITLE);
    setWindowIcon(QIcon(GAME_ICON));
    m_Timer.setInterval(GAME_RATE);
    m_difficultyTimer.setInterval(30000);  // 30秒增加一次难度

    // 时间间隔初始化
    m_recorder = 0;
    m_propRecorder = 0;
    m_enemySpeedIncrement = 0;

    // 初始化道具数组
    for (int i = 0; i < PROP_NUM; i++) {
        m_props[i] = nullptr;
    }
    srand((unsigned int)time(NULL));
    setMouseTracking(true);
}

void MainScene::playGame()
// AI辅助痕迹：此处参考了AI生成的代码。
// 我们甄别并修正了AI代码中直接启动定时器的隐患。手动添加了stop() 操作，防止在玩家重复点击“开始”时出现多个进程
{

    if (m_Timer.isActive()) {
        m_Timer.stop();
    }
    if (m_difficultyTimer.isActive()) {
        m_difficultyTimer.stop();
    }

    // 启动主游戏定时器
    m_Timer.start();

    // 启动动态难度定时器
    m_difficultyTimer.start();

    // 连接定时器信号
    connect(&m_Timer, &QTimer::timeout, [=]() {
        // 如果游戏结束，不更新
        if (!m_isInGame || m_showGameOver) return;

        // 敌机出场
        enermyToScene();

        // 生成道具
        generateProp();

        // 更新坐标
        updatePosition();

        // 更新护盾状态
        m_hero.updateShield();

        // 碰撞检测
        collisionDetection();

        // 检查道具碰撞
        checkPropCollision();

        // 绘制到屏幕
        update();
    });

    connect(&m_difficultyTimer, &QTimer::timeout, this, &MainScene::updateEnemySpeed);
}

void MainScene::startNewGame(int difficulty)
{
    qDebug() << "开始新游戏，难度:" << difficulty;

    // 重置游戏状态
    m_gameState->reset();
    m_gameState->setDifficulty(difficulty);

    // 重置飞机
    m_hero = HeroPlane();

    // 重置敌机
    for (int i = 0; i < ENEMY_NUM; i++) {
        m_enermys[i].m_Free = true;
        if (difficulty == EASY) {
            m_enermys[i].m_Speed = ENEMY_SPEED;
        } else {
            m_enermys[i].m_Speed = ENEMY_SPEED + 2;
        }
    }

    // 重置爆炸效果
    for (int i = 0; i < BOMB_NUM; i++) {
        m_bombs[i].m_Free = true;
    }

    // 重置道具
    for (int i = 0; i < PROP_NUM; i++) {
        if (m_props[i]) {
            delete m_props[i];
            m_props[i] = nullptr;
        }
    }

    // 重置计数器
    m_recorder = 0;
    m_propRecorder = 0;
    m_enemySpeedIncrement = 0;

    // 设置游戏状态
    m_isInGame = true;
    m_showStartMenu = false;
    m_showGameOver = false;

    // 启动游戏
    playGame();
}

void MainScene::updatePosition()
{
    // 更新地图
    m_map.mapPosition();

    // 发射子弹
    m_hero.shoot();

    // 子弹坐标
    for (int i = 0; i < BULLET_NUM; i++) {
        if (m_hero.m_bullets[i].m_Free == false) {
            m_hero.m_bullets[i].updatePosition();
        }
    }

    // 敌机
    for (int i = 0; i < ENEMY_NUM; i++) {
        if (m_enermys[i].m_Free == false) {
            m_enermys[i].updatePosition();
        }
    }

    // 更新道具位置
    updateProps();

    // 爆炸
    for (int i = 0; i < BOMB_NUM; i++) {
        if (m_bombs[i].m_Free == false) {
            m_bombs[i].updateInfo();
        }
    }
}

void MainScene::mousePressEvent(QMouseEvent *event)

{
    if (event->button() == Qt::LeftButton) {
        QPoint clickPos = event->pos();

        qDebug() << "鼠标点击位置: " << clickPos;

        if (m_showStartMenu) {
            // 开始菜单点击检测
            if (m_easyButtonRect.contains(clickPos)) {
                qDebug() << "点击了简单模式按钮";
                startNewGame(EASY);
                update();
                return;
            } else if (m_hardButtonRect.contains(clickPos)) {
                qDebug() << "点击了困难模式按钮";
                startNewGame(HARD);
                update();
                return;
            }
        } else if (m_showGameOver) {
            // 游戏结束界面点击检测
            if (m_restartButtonRect.contains(clickPos)) {
                qDebug() << "点击了重新开始按钮";
                m_showStartMenu = true;
                m_showGameOver = false;
                m_isInGame = false;

                // 停止定时器
                m_Timer.stop();
                m_difficultyTimer.stop();

                update();
                return;
            }
        }
    }

    // 调用父类处理其他事件
    QWidget::mousePressEvent(event);
}

void MainScene::mouseMoveEvent(QMouseEvent *event)
//【AI辅助痕迹】：此处参考了AI生成的鼠标坐标同步逻辑。
// 我门修改了边界检测算法（Clamping），手动加入了对GAME_WIDTH和GAME_HEIGHT的动态判定，确保我们的飞机在高速移动下也不会脱离屏幕，提升了稳定性 。
{
    // 只有在游戏中且飞机存活时才处理飞机移动
    if (m_isInGame && m_hero.isAlive() && !m_showGameOver) {
        QPoint mousePos = event->pos();

        // 计算飞机新位置
        int x = mousePos.x() - m_hero.m_Rect.width() * 0.5;
        int y = mousePos.y() - m_hero.m_Rect.height() * 0.5;

        // 边界检测
        if (x < 0) x = 0;
        if (x > GAME_WIDTH - m_hero.m_Rect.width())
            x = GAME_WIDTH - m_hero.m_Rect.width();
        if (y < 0) y = 0;
        if (y > GAME_HEIGHT - m_hero.m_Rect.height())
            y = GAME_HEIGHT - m_hero.m_Rect.height();

        m_hero.setPosition(x, y);
    }

    if (m_showStartMenu) {
        QPoint pos = event->pos();
        if (m_easyButtonRect.contains(pos) || m_hardButtonRect.contains(pos)) {
            setCursor(Qt::PointingHandCursor);
        } else if (m_showGameOver && m_restartButtonRect.contains(pos)) {
            setCursor(Qt::PointingHandCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }
    }

    // 调用父类处理
    QWidget::mouseMoveEvent(event);
}

// 绘制
void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_showStartMenu) {
        // 绘制开始菜单
        drawStartMenu(&painter);
    } else if (m_showGameOver) {
        // 绘制游戏结束界面
        drawGameOver(&painter);
    } else if (m_isInGame) {
        // 绘制游戏界面
        // 绘制地图
        painter.drawPixmap(0, m_map.m_map1_posY, m_map.m_map1);
        painter.drawPixmap(0, m_map.m_map2_posY, m_map.m_map2);

        // 绘制飞机
        painter.drawPixmap(m_hero.m_X, m_hero.m_Y, m_hero.m_Plane);

        // 绘制护盾
        if (m_hero.hasShield()) {
            painter.setPen(QPen(QColor(0, 255, 255, 180), 3, Qt::DashLine));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(m_hero.m_X - 10, m_hero.m_Y - 10,
                                m_hero.m_Rect.width() + 20, m_hero.m_Rect.height() + 20);
        }

        // 绘制子弹
        for (int i = 0; i < BULLET_NUM; i++) {
            if (m_hero.m_bullets[i].m_Free == false) {
                painter.drawPixmap(m_hero.m_bullets[i].m_X,
                                   m_hero.m_bullets[i].m_Y,
                                   m_hero.m_bullets[i].m_Bullet);
            }
        }

        // 绘制敌机
        for (int i = 0; i < ENEMY_NUM; i++) {
            if (m_enermys[i].m_Free == false) {
                painter.drawPixmap(m_enermys[i].m_X,
                                   m_enermys[i].m_Y,
                                   m_enermys[i].m_enermy);
            }
        }

        // 绘制道具
        for (int i = 0; i < PROP_NUM; i++) {
            if (m_props[i] && !m_props[i]->m_free) {
                painter.drawPixmap(m_props[i]->m_X,
                                   m_props[i]->m_Y,
                                   m_props[i]->m_pixmap);

                painter.setPen(QPen(QColor(255, 255, 0, 100), 2));
                painter.setBrush(Qt::NoBrush);
                painter.drawEllipse(m_props[i]->m_X - 5, m_props[i]->m_Y - 5,
                                    m_props[i]->m_pixmap.width() + 10,
                                    m_props[i]->m_pixmap.height() + 10);
            }
        }

        // 绘制爆炸
        for (int i = 0; i < BOMB_NUM; i++) {
            if (m_bombs[i].m_Free == false) {
                painter.drawPixmap(m_bombs[i].m_X,
                                   m_bombs[i].m_Y,
                                   m_bombs[i].m_pixArr[m_bombs[i].m_index]);
            }
        }

        drawUI(&painter);
    }
}

void MainScene::drawStartMenu(QPainter* painter)// 【AI辅助痕迹】：此处UI背景的渐变渲染参考了AI生成的QLinearGradient代码段。
{
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(30, 30, 100));
    gradient.setColorAt(0.5, QColor(20, 20, 60));
    gradient.setColorAt(1, QColor(10, 10, 30));
    painter->fillRect(rect(), gradient);

    painter->setRenderHint(QPainter::Antialiasing);


    painter->setBrush(QColor(100, 200, 100, 200));
    painter->setPen(QPen(Qt::white, 2));
    painter->drawRoundedRect(m_easyButtonRect, 10, 10);

    painter->setPen(Qt::white);
    painter->setFont(QFont("微软雅黑", 16, QFont::Bold));
    painter->drawText(m_easyButtonRect, Qt::AlignCenter, "简单模式");

    painter->setBrush(QColor(200, 100, 100, 200));
    painter->setPen(QPen(Qt::white, 2));
    painter->drawRoundedRect(m_hardButtonRect, 10, 10);

    painter->setPen(Qt::white);
    painter->drawText(m_hardButtonRect, Qt::AlignCenter, "困难模式");

    painter->setFont(QFont("微软雅黑", 12));
    painter->setPen(QColor(200, 200, 255));
    painter->drawText(rect().adjusted(20, 400, -20, -100),
                      Qt::AlignCenter,
                      "游戏说明：\n"
                      "鼠标移动控制飞机\n"
                      "自动发射子弹\n"
                      "可收集道具：\n"
                      "护盾：抵挡一次伤害\n"
                      "血瓶：增加生命值\n"
                      "击中敌机获得积分");

    // 最高分显示
    painter->setPen(QColor(255, 255, 100));
    painter->drawText(20, height() - 60,
                      QString("简单模式最高分: %1").arg(m_gameState->getHighScore(EASY)));
    painter->drawText(20, height() - 30,
                      QString("困难模式最高分: %1").arg(m_gameState->getHighScore(HARD)));

}

void MainScene::drawUI(QPainter* painter)
{
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 12, QFont::Bold));

    painter->setBrush(QColor(0, 0, 0, 150));
    painter->setPen(Qt::NoPen);
    painter->drawRect(0, 0, 180, 100);

    painter->setPen(Qt::white);

    painter->drawText(10, 25,
                      QString("击中敌机: %1").arg(m_gameState->getHitCount()));


    QString lifeStr;
    for (int i = 0; i < m_hero.getLife(); i++) {
        lifeStr += "❤ ";
    }
    painter->drawText(10, 50, QString("生命: %1").arg(lifeStr));

    QString difficultyStr = (m_gameState->getDifficulty() == EASY) ? "简单" : "困难";
    painter->drawText(width() - 150, 25,
                      QString("难度: %1").arg(difficultyStr));
    painter->drawText(width() - 150, 50,
                      QString("积分: %1").arg(m_gameState->getScore()));

    if (m_hero.hasShield()) {
        int seconds = m_hero.getShieldTime() / 1000;
        painter->setPen(QColor(0, 255, 255));
        painter->drawText(10, height() - 10,
                          QString("护盾剩余: %1秒").arg(seconds));
    }

    // 绘制进度条
    int highScore = m_gameState->getHighScore(m_gameState->getDifficulty());
    if (highScore > 0) {
        int progressWidth = 200;
        int progress = qMin(100, (m_gameState->getScore() * 100) / highScore);

        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(100, 100, 100, 200));
        painter->drawRect(width()/2 - progressWidth/2, 10, progressWidth, 15);

        painter->setBrush(QColor(255, 200, 0, 200));
        painter->drawRect(width()/2 - progressWidth/2, 10,
                          (progressWidth * progress) / 100, 15);

        painter->setPen(Qt::white);
        painter->setFont(QFont("Arial", 9));
        painter->drawText(width()/2 - progressWidth/2, 10, progressWidth, 15,
                          Qt::AlignCenter,
                          QString("%1 / %2").arg(m_gameState->getScore()).arg(highScore));
    }
}

void MainScene::drawGameOver(QPainter* painter)
{
    // 半透明黑色背景
    painter->fillRect(rect(), QColor(0, 0, 0, 220));

    painter->setRenderHint(QPainter::Antialiasing);

    // 游戏结束文字
    painter->setPen(QColor(255, 100, 100));
    painter->setFont(QFont("微软雅黑", 36, QFont::Bold));
    painter->drawText(width()/2 - 150, height()/2 - 150, 300, 80,
                      Qt::AlignCenter, "游戏结束");

    // 显示分数和难度
    painter->setPen(QColor(255, 255, 200));
    painter->setFont(QFont("微软雅黑", 18));

    QString difficultyStr = (m_gameState->getDifficulty() == EASY) ? "简单模式" : "困难模式";
    int highScore = m_gameState->getHighScore(m_gameState->getDifficulty());

    QString scoreText = QString("难度: %1\n\n"
                                "最终积分: %2\n"
                                "击中敌机: %3\n\n"
                                "最高分: %4\n")
                            .arg(difficultyStr)
                            .arg(m_gameState->getScore())
                            .arg(m_gameState->getHitCount())
                            .arg(highScore);

    painter->drawText(width()/2 - 200, height()/2 - 60, 400, 200,
                      Qt::AlignCenter, scoreText);

    // 重新开始按钮
    painter->setBrush(QColor(100, 150, 255, 200));
    painter->setPen(QPen(Qt::white, 2));
    painter->drawRoundedRect(m_restartButtonRect, 10, 10);

    painter->setPen(Qt::white);
    painter->setFont(QFont("微软雅黑", 16, QFont::Bold));
    painter->drawText(m_restartButtonRect, Qt::AlignCenter, "返回主菜单");
}

// 碰撞检测函数需要添加游戏结束逻辑
void MainScene::collisionDetection()
// AI辅助痕迹：此处的双重循环碰撞轮询参考了AI提供的QRect::intersects算法思路。
// 我根据本项目设计的“对象池”原则，手动添加了m_Free状态拦截
{

    for (int i = 0; i < ENEMY_NUM; i++) {
        if (m_enermys[i].m_Free) {
            continue;
        }

        // 检查
        if (m_enermys[i].m_Rect.intersects(m_hero.m_Rect)) {
            m_enermys[i].m_Free = true;

            // 爆炸
            for (int k = 0; k < BOMB_NUM; k++) {
                if (m_bombs[k].m_Free) {
                    m_bombs[k].m_Free = false;
                    m_bombs[k].m_X = m_enermys[i].m_X;
                    m_bombs[k].m_Y = m_enermys[i].m_Y;
                    break;
                }
            }

            m_hero.takeDamage();

            if (!m_hero.isAlive()) {
                m_showGameOver = true;
                m_isInGame = false;
                m_gameState->setGameOver(true);

                m_Timer.stop();
                m_difficultyTimer.stop();

                // 更新界面
                update();
                return;
            }
        }

        for (int j = 0; j < BULLET_NUM; j++) {
            if (m_hero.m_bullets[j].m_Free) {
                continue;
            }

            // 子弹与敌机碰撞
            if (m_enermys[i].m_Rect.intersects(m_hero.m_bullets[j].m_Rect)) {
                m_enermys[i].m_Free = true;
                m_hero.m_bullets[j].m_Free = true;

                // 更新
                m_gameState->addScore(1);
                m_gameState->addHitCount();
                for (int k = 0; k < BOMB_NUM; k++) {
                    if (m_bombs[k].m_Free) {
                        m_bombs[k].m_Free = false;
                        m_bombs[k].m_X = m_enermys[i].m_X;
                        m_bombs[k].m_Y = m_enermys[i].m_Y;
                        break;
                    }
                }
                break;
            }
        }
    }
}

void MainScene::updateProps()
{
    for (int i = 0; i < PROP_NUM; i++) {
        if (m_props[i] && !m_props[i]->m_free) {
            m_props[i]->updatePosition();
        }
    }
}

void MainScene::generateProp()
// AI辅助痕迹：此处道具生成的逻辑由AI辅助构建。
// 我们拒绝了AI直接在主场景new对象的建议，转而应用了工厂模式
{
    m_propRecorder++;
    if (m_propRecorder < PROP_INTERVAL) {
        return;
    }

    m_propRecorder = 0;

    // 随机生成道具
    int propType = rand() % 2;

    for (int i = 0; i < PROP_NUM; i++) {
        if (!m_props[i] || m_props[i]->m_free) {
            if (m_props[i]) {
                delete m_props[i];
            }
            m_props[i] = PropFactory::createProp(propType,
                                                 rand() % (GAME_WIDTH - 50),
                                                 -50);
            break;
        }
    }
}

void MainScene::checkPropCollision()
{
    if (!m_hero.isAlive()) return;

    for (int i = 0; i < PROP_NUM; i++) {
        if (m_props[i] && !m_props[i]->m_free) {
            if (m_hero.m_Rect.intersects(m_props[i]->getRect())) {
                switch (m_props[i]->m_type) {
                case PropFactory::SHIELD:
                    m_hero.activateShield();
                    break;
                case PropFactory::HEART:
                    m_hero.increaseLife();
                    break;
                }

                // 移除道具
                m_props[i]->m_free = true;
            }
        }
    }
}

void MainScene::enermyToScene()
{
    m_recorder++;
    if (m_recorder < ENEMY_INTERVAL) {
        return;
    }

    m_recorder = 0;

    // 遍历非空闲
    for (int i = 0; i < ENEMY_NUM; i++) {
        if (m_enermys[i].m_Free) {
            m_enermys[i].m_Free = false;
            m_enermys[i].m_X = rand() % (GAME_WIDTH - m_enermys[i].m_Rect.width());
            m_enermys[i].m_Y = -m_enermys[i].m_Rect.height();
            break;
        }
    }
}

void MainScene::updateEnemySpeed()
{
    m_enemySpeedIncrement++;

    // 更新速度
    for (int i = 0; i < ENEMY_NUM; i++) {
        m_enermys[i].m_Speed = ENEMY_SPEED + m_enemySpeedIncrement;
    }

    qDebug() << "敌机速度增加至:" << ENEMY_SPEED + m_enemySpeedIncrement;
}
