#include "Welcome.h"
#include "SimpleAudioEngine.h"
#include "SelectScene.h"

using namespace CocosDenshion;

USING_NS_CC;

Scene* Welcome::createScene() {
	//��������
	auto scene = Scene::create();
	//����ͼ��,������õ�create����������Welcomeͷ�ļ������CREATE_FUNC(Welcome);
	auto layer = Welcome::create();
	//��ͼ����ӵ�����
	scene->addChild(layer);
	//���س���
	return scene;
}

bool Welcome::init() {
	//�����еĳ�ʼ������
	if (!Layer::init()) {
		return false;
	}
	//Ԥ���ر������ֺ���Ч
	//SimpleAudioEngine::getInstance()->preloadBackgroundMusic("");
	//SimpleAudioEngine::getInstance()->preloadEffect("");
	//����ͼƬ��Դ
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images.plist");
	//������������
	//����֡��������������
	auto bg = CCSprite::create("BattleCity.png");
	//��ȡ��Ļ��С
	auto size = Director::getInstance()->getWinSize();
	//�������ű���
	//float scaleX = size.width / bg->getContentSize().width;
	//float scaleY = size.height / bg->getContentSize().height;
	//���ñ���������
	//bg->setScaleX(scaleX);
	//bg->setScaleY(scaleY);
	//���ñ�����λ��
	bg->setPosition(Vec2(size.width / 2, size.height - bg->getContentSize().height));
	//��ӵ���ǰͼ��
	this->addChild(bg, 0);
	//�����˵�
	//�˵���1
	auto startItem = MenuItemFont::create("START", CC_CALLBACK_0(Welcome::start, this));
	startItem->setFontSizeObj(20);
	//startItem->setFontNameObj("Verdana-Bold");
	//�˵���2
	auto constructionItem = MenuItemFont::create("CONSTRUCTION", CC_CALLBACK_0(Welcome::construction, this));
	constructionItem->setFontSizeObj(20);
	//constructionItem->setFontNameObj("Verdana-Bold");
	//�˵���3
	auto highscoresItem = MenuItemFont::create("HIGH SCORES", CC_CALLBACK_0(Welcome::highscores, this));
	highscoresItem->setFontSizeObj(20);
	//highscoresItem->setFontNameObj("Verdana-Bold");
	//�˵���4
	auto settingItem = MenuItemFont::create("SETTING", CC_CALLBACK_0(Welcome::set, this));
	settingItem->setFontSizeObj(20);
	//settingItem->setFontNameObj("Verdana-Bold");
	//�˵���5
	auto helpItem = MenuItemFont::create("HELP", CC_CALLBACK_0(Welcome::help, this));
	helpItem->setFontSizeObj(20);
	//helpItem->setFontNameObj("Verdana-Bold");

	//���ݲ˵�����˵�
	auto menu = Menu::create(startItem, constructionItem, highscoresItem, settingItem, helpItem, NULL);
	//��ֱ����
	menu->alignItemsVertically();
	menu->setPosition(Vec2(size.width / 2, size.height - bg->getContentSize().height*2-20));
	//��ӵ�ͼ��
	this->addChild(menu);
	return true;
}

//������Ϸ������
void Welcome::start() {
	//����Ϸ�������滻��ǰ���������Ҿ��г���ת����Ч
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