//
//  HUDLayer.h
//  MyGame
//
//  Created by amaker on 14-6-27.
//
//

#ifndef __MyGame__HUDLayer__
#define __MyGame__HUDLayer__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
// 继承Layer
class HUDLayer : public cocos2d::Layer{
public:
    // 积分标签
    cocos2d::Label *scoreLabel;
    // 玩家生命数精灵集合
    cocos2d::Vector<cocos2d::Sprite*> lifes;
    // 初始化积分标签
    void initLabel();
    // 初始化生命数
    void initLifes();
    // 更新积分
    void updateScore();
    // 初始化
    virtual bool init();
    CREATE_FUNC(HUDLayer);
private:
};
#endif /* defined(__MyGame__HUDLayer__) */
