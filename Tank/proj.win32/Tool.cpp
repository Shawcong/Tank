#include "Tool.h"

using namespace cocos2d;
USING_NS_CC;

Tool* Tool::createWithSpriteFrameName(const char* spriteFrameName) {
	Tool *tool = new Tool();
	if (tool && tool->initWithSpriteFrameName(spriteFrameName)) {
		tool->autorelease();
		//tank->setVisible(false);
		return tool;
	}
	CC_SAFE_DELETE(tool);
	return nullptr;
}

bool Tool::checkcrash(float x, float y, float xxb, float xxc) {
	log("check!!!!!!!!!!!!!!!!");
	if (Rect(this->getPosition().x, this->getPosition().y, this->getContentSize().width, this->getContentSize().height).intersectsRect(Rect(x, y, xxb, xxc))) {
		return true;
	}
	else {
		return false;
	}
}
