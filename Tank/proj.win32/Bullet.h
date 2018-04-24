#pragma once
#include "cocos2d.h"

class Bullet :public cocos2d::Sprite {
	CC_SYNTHESIZE(cocos2d::Vec2, velocity, Velocity);
public:

	void shootBulletFromFighter(Sprite* player);

	virtual void update(float dt);
	void setPosition2(std::string direction_bullet,Sprite *_player);
	static Bullet* createWithSpriteFrameName(std::string spriteFrameName);
	void Bullet::move(std::string direction_bullet);
};