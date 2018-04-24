#include "HUDLayer.h"
#include "Tank.h"


Scene* HUDLayer::createScene() {
	//��������
	auto scene = Scene::create();
	//����ͼ��,������õ�create����������Welcomeͷ�ļ������CREATE_FUNC(Welcome);
	auto layer = HUDLayer::create();
	//��ͼ����ӵ�����
	scene->addChild(layer);
	return scene;
}


bool HUDLayer::init() {
	//�����еĳ�ʼ������
	if (!Layer::init()) {
		return false;
	}
	//��ȡ��Ļ��С
	auto size = Director::getInstance()->getWinSize();
	//����Scroe�ı�ǩ
	auto score = LabelTTF::create("Score:", "Arial", 15);
	score->setPosition(Vec2(size.width - score->getContentSize().width*2, size.height - score->getContentSize().height/2));
	score->setFontFillColor(ccColor3B(0, 0, 0));
	this->addChild(score);
	//���÷����ı�ǩ
	score_num = LabelTTF::create("0", "Arial", 15);
	score_num->setPosition(Vec2(score->getPosition().x+score->getContentSize().width,score->getPosition().y));
	score_num->setFontFillColor(ccColor3B(0, 0, 0));
	this->addChild(score_num, 0, 40);

	// �жϵ������Ƿ����0
	if (enemy_lifes.size()>0) {
		// ѭ��ɾ��
		for (auto l : enemy_lifes) {
			l->removeFromParentAndCleanup(true);
		}
		enemy_lifes.clear();
	}
	for (int i = 0; i < Enemy_num-3; i++) {
		auto life = Sprite::createWithSpriteFrameName("life.png");
		if(i%2==0)
			life->setPosition(Vec2(size.width- score->getContentSize().width * 2, size.height - score->getContentSize().height *2-life->getContentSize().height*(i/2)));
		else
			life->setPosition(Vec2(size.width - score->getContentSize().width * 2+life->getContentSize().width, size.height - score->getContentSize().height * 2 - life->getContentSize().height*(i-1)/2));
		enemy_lifes.pushBack(life);
		this->addChild(life);
	}
	//����HP��ǩ
	auto hp = LabelTTF::create("HP", "Arial", 15);
	hp->setPosition(Vec2(size.width - score->getContentSize().width * 2, size.height/2));
	hp->setFontFillColor(ccColor3B(0, 0, 0));
	this->addChild(hp);
	//����HPͼƬ
	auto hp_pic = CCSprite::createWithSpriteFrameName("hp.png");
	hp_pic->setPosition(Vec2(size.width - score->getContentSize().width * 2, size.height / 2 - hp->getContentSize().height-10));
	this->addChild(hp_pic);
	//����HP��ǩ
	hp_lab = LabelTTF::create(Value(my_lifes).asString(), "Arial", 15);
	hp_lab->setPosition(Vec2(size.width - score->getContentSize().width * 2 + hp_pic->getContentSize().width, size.height / 2 - hp->getContentSize().height - 10));
	hp_lab->setFontFillColor(ccColor3B(0, 0, 0));
	this->addChild(hp_lab);
	//����levelͼƬ
	auto level_pic = CCSprite::createWithSpriteFrameName("level.png");
	level_pic->setPosition(Vec2(size.width - score->getContentSize().width * 2, level_pic->getContentSize().height*2));
	this->addChild(level_pic);
	//����level��ǩ
	auto level_lab = LabelTTF::create("1", "Arial", 15);
	level_lab->setPosition(Vec2(size.width - score->getContentSize().width * 2 + level_pic->getContentSize().width, level_pic->getContentSize().height * 2));
	level_lab->setFontFillColor(ccColor3B(0, 0, 0));
	this->addChild(level_lab);
	//���ñ���
	auto tileMap = TMXTiledMap::create("map2.tmx");
	auto background = CCSprite::create("background.png");
	background->setAnchorPoint(ccp(0, 0.5));
	background->setPosition(Vec2(tileMap->getContentSize().width, size.height/2));
	this->addChild(background,-1);
	return true;
}

void HUDLayer::poptank() {
	removeChild(enemy_lifes.back());
	enemy_lifes.popBack();
}

void HUDLayer::updatescore(int score) {
	__String *str = __String::createWithFormat("%d",score);
	score_num->setString(str->getCString());
}

void HUDLayer::updatelife() {
	my_lifes--;
	__String *str = __String::createWithFormat("%d", my_lifes);
	hp_lab->setString(str->getCString());
}