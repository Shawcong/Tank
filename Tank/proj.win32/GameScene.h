#pragma once
#include "cocos2d.h"
#include "Tank.h"
#include "Tool.h"
#include "HUDLayer.h"
//�����ڵ����ٶ�
#define GameSceneBulletVelocity	120
#define GameSceneNodeBatchTagBullet 900
#define GameSceneNodeBatchTagEnemy 500
#define GameSceneNodeTagFighter 600
#define GameSceneNodeBatchTagEnemyBullet 700
#define GameSceneNodeTagHome 465
#define GameSceneNodeTagTool 200
USING_NS_CC;

//�̳�Layerͼ����
class GameScene : public cocos2d::Layer {
public:
	//��������
	int num = 0;
	static cocos2d::Scene* createScene();
	Tank *_player;
	//����̹��
	int score = 0;
	Vector<Tank*> Enemy;
	TMXTiledMap* tileMap;
	TMXObjectGroup * group;
	cocos2d::TMXLayer *ironbase,*base;
	cocos2d::TMXLayer* background;
	bool ispressing = false;
	static HUDLayer *hudLayer;
	EventKeyboard::KeyCode k;
	int _speed = 5;
	int sssss=0;
	__Array *path;
	int keys[2] = {0,0};//�������˳��,0123-��������
	std::string direction = "u";
	std::string direction_bullet = "u";
	bool up = true, down = true, left = true, right = true;
	std::string direction_move;
	Tank *en1,*en2,*en3;
	Tool *tool=NULL;
	//Tool *tool;
	cocos2d::EventListenerPhysicsContact *contactListener;
	float x_home;
	float y_home;
	//��ʼ��
	virtual bool init();
	//����
	void start();
	void onEnter();
	void movedown(float dt);
	void moveup(float dt);
	void moveleft(float dt);
	void moveright(float dt);
	void move(float dt);
	int Ran();
	Point tileCoordFromPosition(Point pos);
	void GameScene::shootBullet(float dt);
	void GameScene::moveBullet(float dt);
	bool shoot=true;
	void GameScene::turnshoot(float dt);
	void GameScene::moveEnemy(float dt);
	void GameScene::enemyBullet(Tank *a);
	void GameScene::explodeBullet(Bullet *bullet);
	void GameScene::bombDone(Node* sender);
	void GameScene::showTank(Tank *tank);
	void GameScene::showDone(Node* sender, Tank *tank);
	void GameScene::showTool(std::string a);
	void GameScene::deleteTool(float dt);
	void GameScene::resetBase(float dt);
	//����ͼ��Layer�ĺ�
	CREATE_FUNC(GameScene);
private:
};

