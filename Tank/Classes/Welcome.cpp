//
//  WelcomeScene.cpp
//  MyGame
//
//  Created by amaker on 14-6-27.
//
//

#include "Welcome.h"
#include "MainGame.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;
using namespace CocosDenshion;

// 创建场景
Scene* Welcome::createScene(){
    // 创建场景
    auto scene = Scene::create();
    // 图层
    auto layer = Welcome::create();
    // 将图层添加到场景
    scene->addChild(layer);
    // 返回场景
    return scene;
}

// 初始化方法
bool Welcome::init(){
    // 1. 父类初始化
    if (!Layer::init()) {
        return false;
    }
    // 2. 预加载背景音乐和音效
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgssmusic.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("fire01.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("explode1.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("explode2.wav");
    
    // 3. 加载图片资源
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plane.plist");
    // 4. 创建背景精灵
    // 根据帧名称创建背景精灵
    auto bg = Sprite::createWithSpriteFrameName("background.png");
    // 获得屏幕大小
    auto size = Director::getInstance()->getWinSize();
    // 计算缩放比例
    float scaleX = size.width/bg->getContentSize().width;
    float scaleY = size.height/bg->getContentSize().height;
    // 设置背景的缩放
    bg->setScaleX(scaleX);
    bg->setScaleY(scaleY);
    // 设置背景的位置
    bg->setPosition(Vec2(size.width/2,size.height/2));
    // 添加到当前图层
    this->addChild(bg,0);
    
    // 5. 创建菜单
    // 菜单项1
    auto startItem = MenuItemFont::create("Start", CC_CALLBACK_0(Welcome::start, this));
    // 菜单项2
    auto settingItem = MenuItemFont::create("Setting", CC_CALLBACK_0(Welcome::set, this));
    // 根据菜单项创建菜单
    auto menu = Menu::create(startItem,settingItem,NULL);
    // 垂直对齐
    menu->alignItemsVertically();
    // 添加到图层
    this->addChild(menu);
    return true;
}
// 进入游戏主界面
void Welcome::start(){
    // 用游戏主场景替换当前场景，并具有场景转换特效
    Director::getInstance()->replaceScene(TransitionPageTurn::create(1, MainGame::createScene(), false));
}
// 游戏设置，音效什么的
void Welcome::set(){
    
}