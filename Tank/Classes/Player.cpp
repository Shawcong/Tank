//
//  Player.cpp
//  MyGame
//
//  Created by amaker on 14-6-28.
//
//

#include "Player.h"
using namespace cocos2d;
// 生命数为5
int Player::lifes = 5;
// 积分默认为0
int Player::scores = 0;
// 构造方法
Player::Player(){
}
// 析构方法
Player::~Player(){
    
}
// 初始化玩家
bool Player::initSprite(){
    if(!Sprite::init()){
        return false;
    }
    // 获得玩家精灵帧
    auto player_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("player.png");
    // 为玩家设置精灵帧
    this->setSpriteFrame(player_frame);
    // 缩放2倍
    this->setScale (2);
    // 获得屏幕尺寸
    auto size = Director::getInstance()->getWinSize();
    // 设置位置
    this->setPosition(Vec2(size.width/2,100));
    return true;
}
// 创建玩家
Player* Player::createPlayer(){
    // 实例化玩家
    Player *player = new Player();
    // 初始化
    player->initSprite();
    // 自动释放内存
    player->autorelease();
    // 返回玩家
    return player;
}
// 返回生命数
int Player::getLifes(){
    return lifes;
}
// 返回积分
int Player::getScores(){
    return scores;
}