#include "SelectScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* SelectScene::createScene() {
	//��������
	auto scene = Scene::create();
	//����ͼ��,������õ�create����������Welcomeͷ�ļ������CREATE_FUNC(Welcome);
	auto layer = SelectScene::create();
	//��ͼ����ӵ�����
	scene->addChild(layer);
	//���س���
	return scene;
}

bool SelectScene::init() {
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
	auto bg = CCSprite::create("stage.png");
	//��ȡ��Ļ��С
	auto size = Director::getInstance()->getWinSize();
	//�������ű���
	//float scaleX = size.width / bg->getContentSize().width;
	//float scaleY = size.height / bg->getContentSize().height;
	//���ñ���������
	//bg->setScaleX(scaleX);
	//bg->setScaleY(scaleY);
	//���ñ�����λ��
	bg->setPosition(Vec2(size.width / 2, size.height /2+ bg->getContentSize().height/2));
	//��ӵ���ǰͼ��
	this->addChild(bg, 0);
	//��ӱ�ǩ
	auto label = LabelTTF::create("1", "Arial", 35);
	label->setPosition(Vec2(size.width / 2 + bg->getContentSize().width/2+10, size.height / 2 + bg->getContentSize().height/2));
	label->setFontFillColor(ccColor3B(252, 252, 0));
	this->addChild(label);
	//��Ӱ�ť
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
	//����Ϸ�������滻��ǰ���������Ҿ��г���ת����Ч
}

void SelectScene::menuItemplaycallback() {
	//����Ϸ�������滻��ǰ���������Ҿ��г���ת����Ч
	Director::getInstance()->replaceScene(TransitionPageTurn::create(1, GameScene::createScene(), false));
}