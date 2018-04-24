#include "Tank.h"
using namespace cocos2d;

Tank* Tank::createWithSpriteFrameName(const char* spriteFrameName)
{
	Tank *tank = new Tank();
	if (tank && tank->initWithSpriteFrameName(spriteFrameName)) {
		tank->autorelease();
		//tank->setVisible(false);
		auto body = PhysicsBody::createBox(tank->getContentSize());
		body->setCategoryBitmask(0x01);			//0001
		body->setCollisionBitmask(0x02);		//0010
		body->setContactTestBitmask(0x01);
		body->setDynamic(true);
		tank->setPhysicsBody(body);
		return tank;
	}
	CC_SAFE_DELETE(tank);
	return nullptr;
}


//·µ»ØÉúÃüÊý
int Tank::getLifes() {
	return lifes;
}

void Tank::setLifes(int lifes) {
	this->lifes = lifes;
}

bool Tank::getHavetool() {
	return havetool;
}

void Tank::setHavetool(bool havetool) {
	this->havetool = havetool;
}

bool Tank::checkcrash(float x, float y,float xxb, float xxc) {
	log("check!!!!!!!!!!!!!!!!");
	if (Rect(this->getPosition().x, this->getPosition().y, this->getContentSize().width, this->getContentSize().height).intersectsRect(Rect(x, y, xxb,xxc))) {
		return true;
	}
	else {
		return false;
	}
}

void Tank::isBullet() {
	if (bullet == NULL) {
		_bullet_direction = _direction;
		bullet = Bullet::createWithSpriteFrameName("bullet_" + _bullet_direction + ".png");
		bullet->setPosition2(_direction, this);
	}
}

void Tank::direction(std::string a) {
	_direction = a;
}

void Tank::movebullet(float dt) {
	bullet->setPosition2(_bullet_direction, this);
}