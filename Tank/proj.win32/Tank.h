#pragma once
#include "cocos2d.h"
#include "Bullet.h"

using namespace cocos2d;
//�̳о�����
class Tank:public Sprite{
public:
	Bullet *bullet = NULL;
	std::string _direction = "d";
	std::string _bullet_direction = "d";
	bool hastool = false;
	void direction(std::string a);
	static Tank* createWithSpriteFrameName(const char* spriteFrameName);
	//���������
	int getLifes();
	void setLifes(int lifes);
	bool checkcrash(float x, float y, float xxb, float xxc);
	void Tank::movebullet(float dt);
	void isBullet();
	bool Tank::getHavetool();
	void Tank::setHavetool(bool havetool);
	int lifes;//������
	bool isShowing = true;
private:

	bool havetool=false;//�Ƿ�ӵ�е���
};