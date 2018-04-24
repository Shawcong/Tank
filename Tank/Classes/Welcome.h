//
//  WelcomeScene.h
//  MyGame
//
//  Created by amaker on 14-6-27.
//
//

#ifndef __MyGame__WelcomeScene__
#define __MyGame__WelcomeScene__

#include <iostream>
#include  "cocos2d.h"
// 继承Layer 图层类
class Welcome : public cocos2d::Layer{
public:
    // 创建场景
    static cocos2d::Scene* createScene();
    // 初始化
    virtual bool init();
    // 启动
    void start();
    // 设置
    void set();
    // 创建图层Layer的宏
    CREATE_FUNC(Welcome);
private:
};

#endif /* defined(__MyGame__WelcomeScene__) */
