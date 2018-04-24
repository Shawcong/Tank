#include "Welcome.h"
#include "SimpleAudioEngine.h"
#include "SelectScene.h"

using namespace CocosDenshion;

USING_NS_CC;

Scene* Welcome::createScene() {
	//创建场景
	auto scene = Scene::create();
	//创建图层,这里调用的create方法就是在Welcome头文件定义的CREATE_FUNC(Welcome);
	auto layer = Welcome::create();
	//将图层添加到场景
	scene->addChild(layer);
	//返回场景
	return scene;
}

bool Welcome::init() {
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
	auto bg = CCSprite::create("BattleCity.png");
	//获取屏幕大小
	auto size = Director::getInstance()->getWinSize();
	//计算缩放比例
	//float scaleX = size.width / bg->getContentSize().width;
	//float scaleY = size.height / bg->getContentSize().height;
	//设置背景的缩放
	//bg->setScaleX(scaleX);
	//bg->setScaleY(scaleY);
	//设置背景的位置
	bg->setPosition(Vec2(size.width / 2, size.height - bg->getContentSize().height));
	//添加到当前图层
	this->addChild(bg, 0);
	//创建菜单
	//菜单项1
	auto startItem = MenuItemFont::create("START", CC_CALLBACK_0(Welcome::start, this));
	startItem->setFontSizeObj(20);
	//startItem->setFontNameObj("Verdana-Bold");
	//菜单项2
	auto constructionItem = MenuItemFont::create("CONSTRUCTION", CC_CALLBACK_0(Welcome::construction, this));
	constructionItem->setFontSizeObj(20);
	//constructionItem->setFontNameObj("Verdana-Bold");
	//菜单项3
	auto highscoresItem = MenuItemFont::create("HIGH SCORES", CC_CALLBACK_0(Welcome::highscores, this));
	highscoresItem->setFontSizeObj(20);
	//highscoresItem->setFontNameObj("Verdana-Bold");
	//菜单项4
	auto settingItem = MenuItemFont::create("SETTING", CC_CALLBACK_0(Welcome::set, this));
	settingItem->setFontSizeObj(20);
	//settingItem->setFontNameObj("Verdana-Bold");
	//菜单项5
	auto helpItem = MenuItemFont::create("HELP", CC_CALLBACK_0(Welcome::help, this));
	helpItem->setFontSizeObj(20);
	//helpItem->setFontNameObj("Verdana-Bold");

	//根据菜单项创建菜单
	auto menu = Menu::create(startItem, constructionItem, highscoresItem, settingItem, helpItem, NULL);
	//垂直对齐
	menu->alignItemsVertically();
	menu->setPosition(Vec2(size.width / 2, size.height - bg->getContentSize().height*2-20));
	//添加到图层
	this->addChild(menu);
	return true;
}

//进入游戏主界面
void Welcome::start() {
	//用游戏主场景替换当前场景，并且具有场景转换特效
	Director::getInstance()->replaceScene(TransitionPageTurn::create(1, SelectScene::createScene(), false));
}

void Welcome::construction() {

}
void Welcome::highscores() {

}
void Welcome::set() {

}
void Welcome::help() {

}