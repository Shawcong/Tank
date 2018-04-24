#include "cocos2d.h"

//继承Layer图层类
class Welcome : public cocos2d::Layer {
public:
	//创建场景
	static cocos2d::Scene* createScene();
	//初始化
	virtual bool init();
	//启动
	void start();
	//设置
	void construction();
	//最高分
	void highscores();
	//设置
	void set();
	//帮助
	void help();
	//创建图层Layer的宏
	CREATE_FUNC(Welcome);
private:
};
