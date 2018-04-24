#include "MainGame.h"
#include "SimpleAudioEngine.h"
#include "HUDLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

// 静态HUD图层
HUDLayer* MainGame::hudLayer = nullptr;
// 构造方法
MainGame::MainGame():playerBulletVelocity(960),enemyBulletVelocity(180),timeElapsed(0){
    
}
// 析构方法
MainGame::~MainGame(){
    
}
// 创建场景
Scene* MainGame::createScene()
{
    // 场景
    auto scene = Scene::create();
    // 游戏图层
    auto layer = MainGame::create();
    // 添加游戏图层到场景
    scene->addChild(layer);
    // HUD图层
    hudLayer = HUDLayer::create();
    // 添加HUD图层到场景
    scene->addChild(hudLayer);
    // 返回场景
    return scene;
}

// 初始化方法
bool MainGame::init()
{
    // 1. 初始化父类
    if ( !Layer::init() )
    {
        return false;
    }
    // 播放背景音乐
    SimpleAudioEngine::getInstance()->playBackgroundMusic("bgmusic.wav",true);
    
    // 注册时间监听器
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* t,Event* e){
        auto point = t->getLocationInView();
        point = Director::getInstance()->convertToGL(point);
        if(player->getBoundingBox().containsPoint(point)){
            isHoldPlayer = true;
        }
        return true;
    };
    
    listener->onTouchMoved = [=](Touch* t,Event* e){
        auto point = t->getLocationInView();
        point = Director::getInstance()->convertToGL(point);
        if (isHoldPlayer) {
            player->setPosition(point);
        }
    };
    
    listener->onTouchEnded = [=](Touch* t,Event* e){
        isHoldPlayer = false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    // 获得屏幕大小
    size = Director::getInstance()->getVisibleSize();
    // 背景图片1
    background1 = Sprite::createWithSpriteFrameName("back_img1.png");
    
    float scaleX = size.width/background1->getContentSize().width;
    float scaleY = size.height/background1->getContentSize().height;
    
    background1->setScaleX(scaleX);
    background1->setScaleY(scaleY);
    
    background1->setAnchorPoint(Vec2(0,0));
    background1->setPosition(Vec2(0,0));
    this->addChild(background1,0);
    
    // 背景图片2
    background2 = Sprite::createWithSpriteFrameName("back_img2.png");
    
    background2->setScaleX(scaleX);
    background2->setScaleY(scaleY);
    
    background2->setAnchorPoint(Vec2(0,0));
    background2->setPosition(Vec2(0,size.height));
    this->addChild(background2,0);
    
    // 创建玩家
    player = Player::createPlayer();
    this->addChild(player);
    
    // 初始化敌人
    initEnemy();
    
    // game over
    gameOverLabel = Label::createWithSystemFont("Game Over!", "Arial", 48);
    gameOverLabel->setPosition(Vec2(size.width/2,size.height/2));
    gameOverLabel->setVisible(false);
    this->addChild(gameOverLabel);
    
    schedule(schedule_selector(MainGame::tick), 0.1);
    schedule(schedule_selector(MainGame::moveBackground));
    
    return true;
}
// 初始化敌人
void MainGame::initEnemy(){
    for (int i = 1; i<=6; i++) {
        __String *s = __String::createWithFormat("e%d.png",i);
        auto enemy_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(s->getCString());
        auto enemy = Sprite::createWithSpriteFrame(enemy_frame);
        enemy->setAnchorPoint(Vec2(0,1));
        enemy->setPosition(-100,size.height-40*i-50);
        enemies.pushBack(enemy);
        this->addChild(enemy,100);
    }
}
// 移动背景
void MainGame::moveBackground(float dt){
    float dif=4;
    background1->setPosition(Vec2(background1->getPositionX(),background1->getPositionY()-dif));
    background2->setPosition(Vec2(background2->getPositionX(),background2->getPositionY()-dif));
    if (background2->getPositionY()<0) {
        float temp=background2->getPositionY()+size.height;
        background1->setPosition(Vec2(background2->getPositionX(),temp));
    }
    if (background1->getPositionY()<0) {
        float temp=background1->getPositionY()+size.height;
        background2->setPosition(Vec2(background1->getPositionX(),temp));
    }
}

// 玩家发射子弹
void MainGame::playerShoot(){
    if(isHoldPlayer){
        // 播放声音
        SimpleAudioEngine::getInstance()->playEffect("playerfire.wav");
        // 子弹动画
        auto playerBullet = Sprite::createWithSpriteFrameName("bullet1.png");
        // 根据玩家位置，确定子弹位置，在玩家顶部
        float x = player->getPositionX();
        float y = player->getPositionY()+player->getContentSize().height;
        playerBullet->setPosition(Vec2(x,y));
        // 添加到图层
        this->addChild(playerBullet);
        // 添加到子弹集合Vector
        playerBullets.pushBack(playerBullet);
        
        // 根据子弹当前位置和速度，确定子弹的运行时间
        float d = (size.height-playerBullet->getPositionY())/playerBulletVelocity;
        // 移动动作
        auto move = MoveTo::create(d, Vec2(x,size.height+5));
        // 子弹越界，删除子弹
        auto cf = CallFunc::create(CC_CALLBACK_0(MainGame::removePlayerBullet, this,playerBullet));
        // 创建序列动作
        auto seq = Sequence::create(move, cf,NULL);
        // 运行动作
        playerBullet->runAction(seq);
        
        // 碰撞检测1-玩家子弹和敌机
        collisionPlayerBulletAndEnemy();
        // 碰撞检测2-玩家和敌机子弹
        collisionPlayerAndEnemyBullet();
    }
}
// 碰撞检测1-玩家子弹和敌机
void MainGame::collisionPlayerBulletAndEnemy(){
    if(enemies.size()>0){
        // 碰撞检测1-玩家子弹和敌机
        Vector<Sprite*> *tempEnemies = new Vector<Sprite*>();
        for (auto e : enemies) {
            for (auto pb: playerBullets) {
                if (e->getBoundingBox().intersectsRect(pb->getBoundingBox())) {
                    tempEnemies->pushBack(e);
                    SimpleAudioEngine::getInstance()->playEffect("explode2.wav");
                    // 更新玩家积分
                    player->scores++;
                    hudLayer->updateScore();
                }
            }
        }
        
        for (int i=0; i<tempEnemies->size(); i++) {
            Sprite *temp = tempEnemies->at(i);
            enemies.eraseObject(temp);
            this->removeChild(temp);
        }
        delete tempEnemies;
    }
}
// 碰撞检测2-玩家和敌机子弹
void MainGame::collisionPlayerAndEnemyBullet(){
    if (enemyBullets.size()>0) {
        Vector<Sprite*> *tempEnemyBullets = new Vector<Sprite*>();
        
        for (auto eb : enemyBullets) {
            if (player->getBoundingBox().intersectsRect(eb->getBoundingBox())) {
                log("collision3...");
                tempEnemyBullets->pushBack(eb);
                SimpleAudioEngine::getInstance()->playEffect("explode01.wav");
                // 生命数减1
                player->lifes--;
                // 重新绘制
                hudLayer->initLifes();
                // game over!
                if (player->lifes<=0) {
                    gameOverLabel->setVisible(true);
                }
            }
        }
        
        for (int i=0; i<tempEnemyBullets->size(); i++) {
            Sprite *temp = tempEnemyBullets->at(i);
            enemyBullets.eraseObject(temp);
            //this->removeChild(temp);
            temp->removeFromParentAndCleanup(true);
        }
        delete tempEnemyBullets;
    }

}
// 敌机运动
void MainGame::enemyMove(float dt){
    // 判断是否还有敌机
    if(enemies.size()>0){
        // 随机选择一个敌机
        int r = rand()%enemies.size();
        auto enemy = enemies.at(r);
        // 判断该敌机是否在执行动作，如果没有执行动作，则执行左右移动动作
        if (enemy->getNumberOfRunningActions()==0) {
            // 向右移动
            auto  enemyMove = MoveBy::create(2, Vec2(size.width+200, 0));
            // 反向移动
            auto  reverseMove = enemyMove->reverse();
            // 动作序列
            auto seq = Sequence::create(enemyMove,reverseMove, NULL);
            // 执行动作
            enemy->runAction(seq);
        }
        // 敌机发射子弹
        enemyShoot(enemy,dt);
    }
}
// 敌机发射子弹
void MainGame::enemyShoot(Sprite* enemy,float dt){
    // 敌机发射子弹
    timeElapsed+=dt;
    if(timeElapsed>0.2){
        timeElapsed = 0.0f;
        auto enemyBullet = Sprite::createWithSpriteFrameName("bullet2.png");
        float x = enemy->getPositionX()+enemy->getContentSize().width/2;
        float y = enemy->getPositionY()-enemy->getContentSize().height;
        enemyBullet->setPosition(Vec2(x,y));
        this->addChild(enemyBullet);
        
        enemyBullets.pushBack(enemyBullet);
        
        float d = (size.height-enemyBullet->getPositionY())/enemyBulletVelocity;
        //log("d=%f",d);
        auto move = MoveTo::create(d, Vec2(x,-10));
        auto cf = CallFunc::create(CC_CALLBACK_0(MainGame::removeEnemyBullet, this,enemyBullet));
        auto seq = Sequence::create(move, cf,NULL);
        enemyBullet->runAction(seq);
    }
}
// 游戏主逻辑
void MainGame::tick(float dt){
    // 玩家发射子弹
    playerShoot();
    // 敌机运动
    enemyMove(dt);
}
// 删除敌机子弹
void MainGame::removeEnemyBullet(Sprite* s){
    if (s->getPositionY()<-5) {
        log("remove...");
        s->removeFromParentAndCleanup(true);
        enemyBullets.eraseObject(s);
    }
}
// 删除玩家子弹
void MainGame::removePlayerBullet(Sprite* s){
    if (s->getPositionY()>size.height) {
        log("remove...");
        s->removeFromParentAndCleanup(true);
        playerBullets.eraseObject(s);
    }
}


