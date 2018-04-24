#pragma once
#include <iostream>
#include "cocos2d.h"

#define Enemy_num 16
using namespace cocos2d;
// �̳�Layer
class HUDLayer : public cocos2d::Layer {
public:
	//�����������鼯��
	cocos2d::Vector<cocos2d::Sprite*> enemy_lifes;
	int my_lifes = 4;
	cocos2d::LabelTTF *score_num;
	cocos2d::LabelTTF *hp_lab;
	//��������
	static cocos2d::Scene* createScene();
	virtual bool init();
	void poptank();
	void updatescore(int score);
	void updatelife();
	CREATE_FUNC(HUDLayer);
private:
};