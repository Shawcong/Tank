#pragma once
#include "cocos2d.h"

using namespace cocos2d;
//�̳о�����

class Tool:public Sprite {
public:
	int num;
	static Tool * createWithSpriteFrameName(const char* spriteFrameName);
	bool Tool::checkcrash(float x, float y, float xxb, float xxc);
private:

};