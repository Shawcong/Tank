//
//  Player.h
//  MyGame
//
//  Created by amaker on 14-6-28.
//
//

#ifndef __MyGame__Player__
#define __MyGame__Player__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
// 继承精灵类Sprite
class Player:public Sprite{
public:
    // 构造函数
    Player();
    // 析构函数
    ~Player();
    
    // 初始化方法
    bool  initSprite();
    // 创建自己方法
    static Player* createPlayer();
    
    // 生命数
    static int lifes;
    // 积分
    static int scores;
    // 获得生命数
    static int getLifes();
    // 获得积分
    static int getScores();
private:
    
};

#endif /* defined(__MyGame__Player__) */
