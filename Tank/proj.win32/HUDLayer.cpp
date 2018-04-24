#include "HUDLayer.h"
#include "Tank.h"


Scene* HUDLayer::createScene() {
	//创建场景
	auto scene = Scene::create();
	//创建图层,这里调用的create方法就是在Welcome头文件定义的CREATE_FUNC(Welcome);
	auto layer = HUDLayer::create();
	//将图层添加到场景
	scene->addChild(layer);
	return scene;
}


bool HUDLayer::init() {
	//父类中的初始化方法
	if (!Layer::init()) {
		return false;
	}
	//获取屏幕大小
	auto size = Director::getInstance()->getWinSize();
	//设置Scroe的标签
	auto score = LabelTTF::create("Score:", "Arial", 15);
	score->setPosition(Vec2(size.width - score->getContentSize().width*2, size.height - score->getContentSize().height/2));
	score->setFontFillColor(ccColor3B(0, 0, 0));
	this->addChild(score);
	//设置分数的标签
	score_num = LabelTTF::create("0", "Arial", 15);
	score_num->setPosition(Vec2(score->getPosition().x+score->getContentSize().width,score->getPosition().y));
	score_num->setFontFillColor(ccColor3B(0, 0, 0));
	this->addChild(score_num, 0, 40);

	// 判断敌人数是否大于0
	if (enemy_lifes.size()>0) {
		// 循环删除
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
	//设置HP标签
	auto hp = LabelTTF::create("HP", "Arial", 15);
	hp->setPosition(Vec2(size.width - score->getContentSize().width * 2, size.height/2));
	hp->setFontFillColor(ccColor3B(0, 0, 0));
	this->addChild(hp);
	//设置HP图片
	auto hp_pic = CCSprite::createWithSpriteFrameName("hp.png");
	hp_pic->setPosition(Vec2(size.width - score->getContentSize().width * 2, size.height / 2 - hp->getContentSize().height-10));
	this->addChild(hp_pic);
	//设置HP标签
	hp_lab = LabelTTF::create(Value(my_lifes).asString(), "Arial", 15);
	hp_lab->setPosition(Vec2(size.width - score->getContentSize().width * 2 + hp_pic->getContentSize().width, size.height / 2 - hp->getContentSize().height - 10));
	hp_lab->setFontFillColor(ccColor3B(0, 0, 0));
	this->addChild(hp_lab);
	//设置level图片
	auto level_pic = CCSprite::createWithSpriteFrameName("level.png");
	level_pic->setPosition(Vec2(size.width - score->getContentSize().width * 2, level_pic->getContentSize().height*2));
	this->addChild(level_pic);
	//设置level标签
	auto level_lab = LabelTTF::create("1", "Arial", 15);
	level_lab->setPosition(Vec2(size.width - score->getContentSize().width * 2 + level_pic->getContentSize().width, level_pic->getContentSize().height * 2));
	level_lab->setFontFillColor(ccColor3B(0, 0, 0));
	this->addChild(level_lab);
	//设置背景
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