#include "Bullet.h"

USING_NS_CC;

Bullet* Bullet::createWithSpriteFrameName(std::string spriteFrameName)
{
	Bullet *bullet = new Bullet();
	if (bullet && bullet->initWithSpriteFrameName(spriteFrameName)) {
		bullet->autorelease();
		//bullet->setVisible(false);
		auto body = PhysicsBody::createBox(bullet->getContentSize());
		body->setCategoryBitmask(0x01);			//0001
		body->setCollisionBitmask(0x02);		//0010
		body->setContactTestBitmask(0x01);
		bullet->setPhysicsBody(body);
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return nullptr;
}

void Bullet::shootBulletFromFighter(Sprite* fighter)
{
	this->setPosition(fighter->getPosition() + Vec2(fighter->getContentSize().width / 2, fighter->getContentSize().height));
	this->setVisible(true);
	this->unscheduleUpdate();
	this->scheduleUpdate();

}

void Bullet::update(float dt)
{
	Size screenSize = Director::getInstance()->getVisibleSize();
	this->setPosition(Vec2(this->getPosition() + velocity *dt));

	if (this->getPosition().y > screenSize.height) {
		//log("isVisible = %d",this->isVisible());
		this->setVisible(false);
		this->unscheduleUpdate();
		this->removeFromParent();
	}
}

void Bullet::setPosition2(std::string direction_bullet,Sprite *_player) {
	if (direction_bullet.compare("u") == 0)
		this->setPosition(_player->getPosition() + Vec2(_player->getContentSize().width / 2, _player->getContentSize().height));
	else if (direction_bullet.compare("l") == 0)
		this->setPosition(_player->getPosition() + Vec2(0, _player->getContentSize().height / 2));
	else if (direction_bullet.compare("r") == 0)
		this->setPosition(_player->getPosition() + Vec2(_player->getContentSize().width, _player->getContentSize().height / 2));
	else if (direction_bullet.compare("d") == 0)
		this->setPosition(_player->getPosition() + Vec2(_player->getContentSize().width / 2, 0));
}

void Bullet::move(std::string direction_bullet) {
	if (direction_bullet.compare("u") == 0)
		this->setPosition(this->getPosition() + Vec2(0, 5));    //每帧移动1
	if (direction_bullet.compare("d") == 0)
		this->setPosition(this->getPosition() + Vec2(0, -5));    //每帧移动1
	if (direction_bullet.compare("l") == 0)
		this->setPosition(this->getPosition() + Vec2(-5, 0));    //每帧移动1
	if (direction_bullet.compare("r") == 0)
		this->setPosition(this->getPosition() + Vec2(5, 0));    //每帧移动1
}