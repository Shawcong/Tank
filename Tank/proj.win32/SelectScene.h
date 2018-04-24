#pragma once
#include "cocos2d.h"

class SelectScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	//Æô¶¯
	void start();
	void menuItemplaycallback();
	// implement the "static create()" method manually
	CREATE_FUNC(SelectScene);
};