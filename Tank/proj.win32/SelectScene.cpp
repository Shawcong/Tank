#include "SelectScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* SelectScene::createScene() {
	//创建场景
	auto scene = Scene::create();
	//创建图层,这里调用的create方法就是在Welcome头文件定义的CREATE_FUNC(Welcome);
	auto layer = SelectScene::create();
	//将图层添加到场景
	scene->addChild(layer);
	//返回场景
	return scene;
}

bool SelectScene::init() {
	//父类中的初始化方法
	if (!Layer::init()) {
		return false;
	}
	//预加载背景音乐和音效
	//SimpleAudioEngine::getInstance()->preloadBackgroundMusic("");
	//SimpleAudioEngine::getInstance()->preloadEffect("");
	//加载图片资源
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images.plist");
	//创建背景精灵
	//根据帧名创建背景精灵
	auto bg = CCSprite::create("stage.png");
	//获取屏幕大小
	auto size = Director::getInstance()->getWinSize();
	//计算缩放比例
	//float scaleX = size.width / bg->getContentSize().width;
	//float scaleY = size.height / bg->getContentSize().height;
	//设置背景的缩放
	//bg->setScaleX(scaleX);
	//bg->setScaleY(scaleY);
	//设置背景的位置
	bg->setPosition(Vec2(size.width / 2, size.height /2+ bg->getContentSize().height/2));
	//添加到当前图层
	this->addChild(bg, 0);
	//添加标签
	auto label = LabelTTF::create("1", "Arial", 35);
	label->setPosition(Vec2(size.width / 2 + bg->getContentSize().width/2+10, size.height / 2 + bg->getContentSize().height/2));
	label->setFontFillColor(ccColor3B(252, 252, 0));
	this->addChild(label);
	//添加按钮
	auto play = CCSprite::create("play.png");
	auto playselected = CCSprite::create("playselected.png");
	MenuItemSprite * playMenuItem = MenuItemSprite::create(play, playselected, CC_CALLBACK_0(SelectScene::menuItemplaycallback, this));
	playMenuItem->setPosition(Vec2(size.width / 2, size.height / 2 - bg->getContentSize().height / 2));
	auto *playMenu = Menu::create(playMenuItem, NULL);
	playMenu->setPosition(Vec2::ZERO);
	this->addChild(playMenu);
	//
	auto before = CCSprite::create("before.png");
	auto beforeselected = CCSprite::create("beforeselected.png");
	MenuItemSprite * beforeMenuItem = MenuItemSprite::create(before, beforeselected, CC_CALLBACK_0(SelectScene::menuItemplaycallback, this));
	beforeMenuItem->setPosition(Vec2(size.width / 2 - play->getContentSize().width, size.height / 2 - bg->getContentSize().height / 2));
	auto *beforeMenu = Menu::create(beforeMenuItem, NULL);
	beforeMenu->setPosition(Vec2::ZERO);
	this->addChild(beforeMenu);
	//
	auto next = CCSprite::create("next.png");
	auto nextselected = CCSprite::create("nextselected.png");
	MenuItemSprite * nextMenuItem = MenuItemSprite::create(next, nextselected, CC_CALLBACK_0(SelectScene::menuItemplaycallback, this));
	nextMenuItem->setPosition(Vec2(size.width / 2 + play->getContentSize().width, size.height / 2 - bg->getContentSize().height / 2));
	auto *nextMenu = Menu::create(nextMenuItem, NULL);
	nextMenu->setPosition(Vec2::ZERO);
	this->addChild(nextMenu);
	return true;
}

void SelectScene::start() {
	//用游戏主场景替换当前场景，并且具有场景转换特效
}

void SelectScene::menuItemplaycallback() {
	//用游戏主场景替换当前场景，并且具有场景转换特效
	Director::getInstance()->replaceScene(TransitionPageTurn::create(1, GameScene::createScene(), false));
}