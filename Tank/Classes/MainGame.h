#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include  "Player.h"
#include "HUDLayer.h"

class MainGame : public cocos2d::Layer
{
public:
    // 创建场景
    static cocos2d::Scene* createScene();
    // 初始化
    virtual bool init();
    // 游戏逻辑
    void tick(float dt);
    // 移动背景
    void moveBackground(float dt);
    // 删除玩家子弹
    void removePlayerBullet(cocos2d::Sprite* s);
    // 删除敌机子弹
    void removeEnemyBullet(cocos2d::Sprite* s);
    // 玩家发射子弹
    void playerShoot();
    // 碰撞检测1-玩家子弹和敌机
    void collisionPlayerBulletAndEnemy();
    // 碰撞检测2-玩家和敌机子弹
    void collisionPlayerAndEnemyBullet();
    // 敌机运动
    void enemyMove(float dt);
    // 敌机发射子弹
    void enemyShoot(Sprite* enemy,float dt);
    // 初始化敌人方法
    void initEnemy();
    // 构造函数
    MainGame();
    // 析构函数
    ~MainGame();
    CREATE_FUNC(MainGame);
private:
    // 游戏结束标签
    Label *gameOverLabel;
    // 判断触屏点是否在Player上面
    bool isHoldPlayer;
    // 玩家对象
    Player *player;
    // 另个滚动背景
    cocos2d::Sprite *background1,*background2;
    // 屏幕大小
    cocos2d::Size size;
    // HUD 图层
    static HUDLayer *hudLayer;
    // 敌人集合
    cocos2d::Vector<cocos2d::Sprite*> enemies;
    // 玩家子弹集合
    cocos2d::Vector<cocos2d::Sprite*> playerBullets;
    // 敌人子弹集合
    cocos2d::Vector<cocos2d::Sprite*> enemyBullets;
    // 玩家子弹速度、敌人子弹速度
    float playerBulletVelocity,enemyBulletVelocity;
    // 消耗的时间
    float timeElapsed;
};

#endif // __HELLOWORLD_SCENE_H__
