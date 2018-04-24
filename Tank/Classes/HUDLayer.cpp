//
//  HUDLayer.cpp
//  MyGame
//
//  Created by amaker on 14-6-27.
//
//

#include "HUDLayer.h"
#include "Player.h"

using namespace cocos2d;
// 初始化方法
bool HUDLayer::init(){
    if(!Layer::init()){
        return false;
    }
    // 初始化标签
    initLabel();
    // 初始化生命数
    initLifes();
    return true;
}
// 初始化标签
void HUDLayer::initLabel(){
    // 创建标签
    scoreLabel = Label::createWithTTF("Score:0","Marker Felt.ttf",24);
    // 设置锚点和位置，并添加到图层
    if (scoreLabel) {
        scoreLabel->setAnchorPoint(Vec2(0,1));
        scoreLabel->setPosition(10,Director::getInstance()->getWinSize().height-10);
        this->addChild(scoreLabel);
    }
}
// 更新积分
void HUDLayer::updateScore(){
    __String *str = __String::createWithFormat("Score:%d",Player::getScores());
    scoreLabel->setString(str->getCString());
}

// 初始化生命数
void HUDLayer::initLifes(){
    // 判断生命数是否大于0
    if (lifes.size()>0) {
        // 循环删除
        for(auto l:lifes){
            l->removeFromParentAndCleanup(true);
        }
        lifes.clear();
    }
    // 获得屏幕尺寸
    auto size = Director::getInstance()->getWinSize();
    // 各个精灵之间的间距
    int gap = 10;
    int life_num = Player::getLifes();
    // 循环初始化
    for (int i=0; i<life_num; i++) {
        auto life = Sprite::createWithSpriteFrameName("life.png");
        life->setAnchorPoint(Vec2(1,1));
        lifes.pushBack(life);
        life->setPosition(size.width-i*gap-gap,size.height-20);
        this->addChild(life);
    }
}