#include "GameScene.h"
#include "Bullet.h"
#include "Welcome.h"
#include "Astar.h"
#include "AstarItem.h"

USING_NS_CC;

//静态HUD图层
HUDLayer* GameScene::hudLayer = nullptr;
Scene* GameScene::createScene() {
	//创建场景
	auto scene = Scene::createWithPhysics();
	PhysicsWorld* phyWorld = scene->getPhysicsWorld();
	//phyWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	//0,0不受到重力的影响
	phyWorld->setGravity(Vect(0, 0));
	//创建图层,这里调用的create方法就是在Welcome头文件定义的CREATE_FUNC(Welcome);
	auto layer = GameScene::create();
	//将图层添加到场景
	scene->addChild(layer);
	//返回场景
	hudLayer = HUDLayer::create(); 
	scene->addChild(hudLayer);
	return scene;
}

bool GameScene::init() {
	//父类中的初始化方法
	if (!Layer::init()) {
		return false;
	}
	//加载图片资源
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images1.plist");
	for (int i=0; i < Enemy_num; i++) {
		Tank* tank;
		tank = Tank::createWithSpriteFrameName("enemy1_d.png");
		tank->setAnchorPoint(ccp(0, 0));
		tank->setLifes(1);
		tank->setHavetool(false);
		Enemy.pushBack(tank);
	}

	//加载瓦片地图
	tileMap = TMXTiledMap::create("map2.tmx");
	tileMap->setAnchorPoint(ccp(0, 0.5));
	ironbase = tileMap->getLayer("ironbase");
	base = tileMap->getLayer("base");
	background = tileMap->getLayer("bg1");
	//tileMap->removeChildByName("ironbase");
	ironbase->setVisible(false);
	//ironbase->removeFromParent();
	//获取屏幕大小
	auto size = Director::getInstance()->getWinSize();
	tileMap->setPosition(Vec2(0, size.height / 2));
	this->addChild(tileMap, -1);
	group = tileMap->getObjectGroup("objects");
	ValueMap spawnPoint = group->getObject("pl1");
	float x = spawnPoint["x"].asFloat();
	float y = spawnPoint["y"].asFloat();
	_player = Tank::createWithSpriteFrameName("tank1_u.png");
	_player->setAnchorPoint(ccp(0, 0));
	_player->setLifes(4);
	_player->setPosition(Vec2(x, y));
	addChild(_player,0, GameSceneNodeTagFighter);

	Sprite *home = Sprite::createWithSpriteFrameName("home.png");
	spawnPoint = group->getObject("home");
	x_home = spawnPoint["x"].asFloat();
	y_home = spawnPoint["y"].asFloat();
	home->setAnchorPoint(ccp(0, 0));
	home->setPosition(Vec2(x_home, y_home));
	auto body = PhysicsBody::createBox(home->getContentSize());
	body->setCategoryBitmask(0x01);			//0001
	body->setCollisionBitmask(0x02);		//0010
	body->setContactTestBitmask(0x01);
	body->setDynamic(true);
	home->setPhysicsBody(body);
	this->addChild(home, 0, GameSceneNodeTagHome);

	spawnPoint = group->getObject("en1");
	float x_en1 = spawnPoint["x"].asFloat();
	float y_en1 = spawnPoint["y"].asFloat();
	spawnPoint = group->getObject("en2");
	float x_en2 = spawnPoint["x"].asFloat();
	float y_en2 = spawnPoint["y"].asFloat();
	spawnPoint = group->getObject("en3");
	float x_en3 = spawnPoint["x"].asFloat();
	float y_en3 = spawnPoint["y"].asFloat();

	en1 = Enemy.back();
	Enemy.popBack();
	en2 = Enemy.back();
	Enemy.popBack();
	en3 = Enemy.back();
	Enemy.popBack();
	en1->setAnchorPoint(ccp(0, 0));
	en1->setPosition(Vec2(x_en1, y_en1));
	
	en2->setAnchorPoint(ccp(0, 0));
	en2->setPosition(Vec2(x_en2, y_en2));
	
	en3->setAnchorPoint(ccp(0, 0));
	en3->setPosition(Vec2(x_en3, y_en3));
	

	Astar *star = new Astar();
	path = star->findPath(x_en1, y_en1, x, y, tileMap, en1);
	path->retain();
	/*en1->setPosition(Vec2(x_en1, y_en1));
	en2->setPosition(Vec2(x_en2, y_en2));
	en3->setPosition(Vec2(x_en3, y_en3));
	this->addChild(en1);
	this->addChild(en2);
	this->addChild(en3);*/
	num += 3;
	return true;
}

void GameScene::onEnter() {
	Layer::onEnter();
	showTank(en1);
	this->addChild(en1, 0, GameSceneNodeBatchTagEnemy);
	showTank(en2);
	this->addChild(en2, 0, GameSceneNodeBatchTagEnemy);
	showTank(en3);
	this->addChild(en3, 0, GameSceneNodeBatchTagEnemy);
	
	auto listener = EventListenerKeyboard::create();
	//注册 接触事件监听器
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [this](PhysicsContact& contact)
	{
		auto spriteA = contact.getShapeA()->getBody()->getNode();
		auto spriteB = contact.getShapeB()->getBody()->getNode();
		////////////////////////////检测 炮弹与敌人的接触 start////////////////////////////////
		if (spriteB!=NULL&&spriteA != NULL && spriteA->getTag() == GameSceneNodeBatchTagBullet &&
			spriteB->getTag() == GameSceneNodeBatchTagEnemy)
		{
			explodeBullet((Bullet *)spriteA);
			removeChild(spriteA);
			score += 100;
			hudLayer->updatescore(score);
			spriteB->setVisible(false);
			if (en1 != NULL&&!en1->isVisible()) {
				if (en1->getLifes() == 1) {//当坦克只有一条命时
					if (num < Enemy_num) {
						if (en1->getHavetool()) {//增加道具
							srand(time(0));
							int tool_num = rand() % 1;
							tool_num++;
							
							//tool = new Tool();
							showTool("tool" + std::to_string(tool_num) + ".png");
							//scheduleOnce(schedule_selector(GameScene::deleteTool), 5.0f);
						}
						removeChild(en1->bullet);
						en1->bullet = NULL;
						if (num % 4 == 0 && num != 0) {
							en1->setLifes(1);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_d.png");
							en1->setDisplayFrame(frame);
							en1->setHavetool(true);
						}
						else if (num % 3 == 0 && num != 0) {
							en1->setLifes(3);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en1->getLifes()) + "_d.png");
							en1->setDisplayFrame(frame);
							en1->setHavetool(false);
						}
						else {
							en1->setHavetool(false);
						}
						ValueMap spawnPoint = group->getObject("en1");
						float x_en1 = spawnPoint["x"].asFloat();
						float y_en1 = spawnPoint["y"].asFloat();
						en1->setPosition(Vec2(x_en1, y_en1));
						en1->setVisible(true);
						showTank(en1);
						num++;
						hudLayer->poptank();
					}
					else {
						if (en1->getHavetool()) {//增加道具
							srand(time(0));
							int tool_num = rand() % 6;
							tool_num++;
							showTool("tool" + std::to_string(tool_num) + ".png");
						}
						removeChild(en1->bullet);
						removeChild(en1);
						en1 = NULL;
					}
				}
				else {//当坦克大于一条命时
					en1->setVisible(true);
					en1->lifes--;
					std::string asd = "enemy";
					asd = asd + std::to_string(en1->getLifes());
					asd = asd + "_";
					asd = asd + en1->_direction;
					asd = asd + ".png";
					CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(asd);
					en1->setDisplayFrame(frame);
				}
			}
			if (en2 != NULL&&!en2->isVisible()) {
				if (en2->getLifes() == 1) {//当坦克只有一条命时
					if (num < Enemy_num) {
						removeChild(en2->bullet);
						en2->bullet = NULL;
						if (num % 4 == 0 && num != 0) {
							en2->setLifes(1);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_d.png");
							en2->setDisplayFrame(frame);
							en2->setHavetool(true);
						}
						else if (num % 3 == 0 && num != 0) {
							en2->setLifes(3);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en2->getLifes()) + "_d.png");
							en2->setDisplayFrame(frame);
							en2->setHavetool(false);
						}
						else {
							en2->setHavetool(false);
						}

						ValueMap spawnPoint = group->getObject("en2");
						float x_en2 = spawnPoint["x"].asFloat();
						float y_en2 = spawnPoint["y"].asFloat();
						en2->setPosition(Vec2(x_en2, y_en2));
						en2->setVisible(true);
						showTank(en2);
						num++;
						hudLayer->poptank();
					}
					else {
						removeChild(en2->bullet);
						removeChild(en2);
						en2 = NULL;
					}
				}
				else {//当坦克大于一条命时
					en2->setVisible(true);
					en2->lifes--;
					std::string asd = "enemy";
					asd = asd + std::to_string(en2->getLifes()) + "_" + en2->_direction + ".png";
					CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(asd);
					en2->setDisplayFrame(frame);
				}
			}
			if (en3 != NULL&&!en3->isVisible()) {
				if (en3->getLifes() == 1) {//当坦克只有一条命时
					if (num < Enemy_num) {
						removeChild(en3->bullet);
						en3->bullet = NULL;
						if (num % 4 == 0 && num != 0) {
							en3->setLifes(1);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_d.png");
							en3->setDisplayFrame(frame);
							en3->setHavetool(true);
						}
						else if (num % 3 == 0 && num != 0) {
							en3->setLifes(3);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en3->getLifes()) + "_d.png");
							en3->setDisplayFrame(frame);
							en3->setHavetool(false);
						}
						else {
							en3->setHavetool(false);
						}

						ValueMap spawnPoint = group->getObject("en3");
						float x_en3 = spawnPoint["x"].asFloat();
						float y_en3 = spawnPoint["y"].asFloat();
						en3->setPosition(Vec2(x_en3, y_en3));
						en3->setVisible(true);
						showTank(en3);
						num++;
						hudLayer->poptank();
					}
					else {
						removeChild(en3->bullet);
						removeChild(en3);
						en3 = NULL;
					}
				}
				else {//当坦克大于一条命时
					en3->setVisible(true);
					en3->lifes--;
					std::string asd = "enemy";
					asd = asd + std::to_string(en3->getLifes()) + "_" + en3->_direction + ".png";
					CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(asd);
					en3->setDisplayFrame(frame);
				}
			}
			if(en1==NULL&&en2==NULL&&en3==NULL)
				Director::getInstance()->replaceScene(TransitionPageTurn::create(1, Welcome::createScene(), false));
			return true;
		}
		else if (spriteB != NULL && spriteA != NULL&&spriteA->getTag() == GameSceneNodeBatchTagEnemy &&
			 spriteB->getTag() == GameSceneNodeBatchTagBullet)
		{
			explodeBullet((Bullet *)spriteB);
			score += 100;
			hudLayer->updatescore(score);
			removeChild(spriteB);
			spriteA->setVisible(false);
			if (en1 != NULL&&!en1->isVisible()) {
				if (en1->getLifes() == 1) {//当坦克只有一条命时
					if (num < Enemy_num) {
						if (en1->getHavetool()) {//增加道具
							srand(time(0));
							int tool_num = rand() % 6;
							tool_num++;
							showTool("tool" + std::to_string(tool_num) + ".png");
						}
						removeChild(en1->bullet);
						en1->bullet = NULL;
						if (num % 4 == 0 && num != 0) {
							en1->setLifes(1);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_d.png");
							en1->setDisplayFrame(frame);
							en1->setHavetool(true);
						}
						else if (num % 3 == 0 && num != 0) {
							en1->setLifes(3);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en1->getLifes()) + "_d.png");
							en1->setDisplayFrame(frame);
							en1->setHavetool(false);
						}
						else {
							en1->setHavetool(false);
						}

						ValueMap spawnPoint = group->getObject("en1");
						float x_en1 = spawnPoint["x"].asFloat();
						float y_en1 = spawnPoint["y"].asFloat();
						en1->setPosition(Vec2(x_en1, y_en1));
						en1->setVisible(true);
						showTank(en1);
						num++;
						hudLayer->poptank();
					}
					else {
						if (en1->getHavetool()) {//增加道具
							srand(time(0));
							int tool_num = rand() % 6;
							tool_num++;
							showTool("tool" + std::to_string(tool_num) + ".png");
						}
						removeChild(en1->bullet);
						removeChild(en1);
						en1 = NULL;
					}
				}
				else {//当坦克大于一条命时
					en1->setVisible(true);
					en1->lifes--;
					std::string asd = "enemy";
					asd = asd + std::to_string(en1->getLifes());
					asd = asd + "_";
					asd = asd + en1->_direction;
					asd = asd + ".png";
					CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(asd);
					en1->setDisplayFrame(frame);
				}
			}
			if (en2 != NULL&&!en2->isVisible()) {
				if (en2->getLifes() == 1) {//当坦克只有一条命时
					if (num < Enemy_num) {
						removeChild(en2->bullet);
						en2->bullet = NULL;
						if (num % 4 == 0 && num != 0) {
							en2->setLifes(1);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_d.png");
							en2->setDisplayFrame(frame);
							en2->setHavetool(true);
						}
						else if (num % 3 == 0 && num != 0) {
							en2->setLifes(3);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en2->getLifes()) + "_d.png");
							en2->setDisplayFrame(frame);
							en2->setHavetool(false);
						}
						else {
							en2->setHavetool(false);
						}

						ValueMap spawnPoint = group->getObject("en2");
						float x_en2 = spawnPoint["x"].asFloat();
						float y_en2 = spawnPoint["y"].asFloat();
						en2->setPosition(Vec2(x_en2, y_en2));
						en2->setVisible(true);
						showTank(en2);
						num++;
						hudLayer->poptank();
					}
					else {
						removeChild(en2->bullet);
						removeChild(en2);
						en2 = NULL;
					}
				}
				else {//当坦克大于一条命时
					en2->setVisible(true);
					en2->lifes--;
					std::string asd = "enemy";
					asd = asd + std::to_string(en2->getLifes()) + "_" + en2->_direction + ".png";
					CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(asd);
					en2->setDisplayFrame(frame);
				}
			}
			if (en3 != NULL&&!en3->isVisible()) {
				if (en3->getLifes() == 1) {//当坦克只有一条命时
					if (num < Enemy_num) {
						removeChild(en3->bullet);
						en3->bullet = NULL;
						if (num % 4 == 0 && num != 0) {
							en3->setLifes(1);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_d.png");
							en3->setDisplayFrame(frame);
							en3->setHavetool(true);
						}
						else if (num % 3 == 0 && num != 0) {
							en3->setLifes(3);
							CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en3->getLifes()) + "_d.png");
							en3->setDisplayFrame(frame);
							en3->setHavetool(false);
						}
						else {
							en3->setHavetool(false);
						}

						ValueMap spawnPoint = group->getObject("en3");
						float x_en3 = spawnPoint["x"].asFloat();
						float y_en3 = spawnPoint["y"].asFloat();
						en3->setPosition(Vec2(x_en3, y_en3));
						en3->setVisible(true);
						showTank(en3);
						num++;
						hudLayer->poptank();
					}
					else {
						removeChild(en3->bullet);
						removeChild(en3);
						en3 = NULL;
					}
				}
				else {//当坦克大于一条命时
					en3->setVisible(true);
					en3->lifes--;
					std::string asd = "enemy";
					asd = asd + std::to_string(en3->getLifes()) + "_" + en3->_direction + ".png";
					CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(asd);
					en3->setDisplayFrame(frame);
				}
			}
			if (en1 == NULL&&en2 == NULL&&en3 == NULL)
				Director::getInstance()->replaceScene(TransitionPageTurn::create(1, Welcome::createScene(), false));
			return true;
		}
		/////////////////////////检测 炮弹与敌人的接触 end/////////////////////////////////////
		////////////////////////////检测 炮弹与自己的接触 start////////////////////////////////
		else if (spriteB != NULL&&spriteA != NULL &&spriteA->getTag() == GameSceneNodeBatchTagEnemyBullet &&
			spriteB->getTag() == GameSceneNodeTagFighter)
		{
			//explodeBullet((Bullet *)spriteA);
			spriteA->setPosition(Vec2(-5,-5));
			_player->lifes--;
			hudLayer->updatelife();
			if(_player->lifes <= 0)
				Director::getInstance()->replaceScene(TransitionPageTurn::create(1, Welcome::createScene(), false));
			else {
				TMXObjectGroup * group = tileMap->getObjectGroup("objects");
				ValueMap spawnPoint = group->getObject("pl1");
				float x = spawnPoint["x"].asFloat();
				float y = spawnPoint["y"].asFloat();
				_player->setPosition(Vec2(x, y));
			}
			return true;
		}
		else if (spriteB != NULL&&spriteA != NULL &&spriteA->getTag() == GameSceneNodeTagFighter &&
			spriteB->getTag() == GameSceneNodeBatchTagEnemyBullet)
		{
			//explodeBullet((Bullet *)spriteB);
			spriteB->setPosition(Vec2(-5, -5));
			_player->lifes--;
			hudLayer->updatelife();
			if (_player->lifes <= 0)
				Director::getInstance()->replaceScene(TransitionPageTurn::create(1, Welcome::createScene(), false));
			else {
				TMXObjectGroup * group = tileMap->getObjectGroup("objects");
				ValueMap spawnPoint = group->getObject("pl1");
				float x = spawnPoint["x"].asFloat();
				float y = spawnPoint["y"].asFloat();
				_player->setPosition(Vec2(x, y));
			}
			return true;
		}
		/////////////////////////检测 炮弹与自己i的接触 end/////////////////////////////////////
		////////////////////////////检测 炮弹与基地的接触 start////////////////////////////////
		else if (spriteB != NULL&&spriteA != NULL &&spriteA->getTag() == GameSceneNodeBatchTagEnemyBullet &&
			spriteB->getTag() == GameSceneNodeTagHome) {
			Director::getInstance()->replaceScene(TransitionPageTurn::create(1, Welcome::createScene(), false));
		}
		else if (spriteB != NULL&&spriteA != NULL &&spriteA->getTag() == GameSceneNodeTagHome
			&& spriteB->getTag() == GameSceneNodeBatchTagEnemyBullet) {
			Director::getInstance()->replaceScene(TransitionPageTurn::create(1, Welcome::createScene(), false));
		}
		else if (spriteB != NULL&&spriteA != NULL &&spriteA->getTag() == GameSceneNodeBatchTagBullet &&
			spriteB->getTag() == GameSceneNodeTagHome) {
			Director::getInstance()->replaceScene(TransitionPageTurn::create(1, Welcome::createScene(), false));
		}
		else if (spriteB != NULL&&spriteA != NULL &&spriteA->getTag() == GameSceneNodeTagHome
			&& spriteB->getTag() == GameSceneNodeBatchTagBullet) {
			Director::getInstance()->replaceScene(TransitionPageTurn::create(1, Welcome::createScene(), false));
		}
		/////////////////////////检测 炮弹与基地的接触 end/////////////////////////////////////
		///////////////////////////检测 炮弹与炮弹的接触 start////////////////////////////////
		else if (spriteB != NULL&&spriteA != NULL &&spriteA->getTag() == GameSceneNodeBatchTagBullet &&spriteB->getTag() == GameSceneNodeBatchTagEnemyBullet) {
			explodeBullet((Bullet *)spriteA);
			removeChild(spriteA);
			spriteB->setVisible(false);
			if (!en1->bullet->isVisible())
				en1->bullet = NULL;
			else if (!en2->bullet->isVisible())
				en2->bullet = NULL;
			else if (!en3->bullet->isVisible())
				en3->bullet = NULL;
			removeChild(spriteB);
			return true;
		}
		else if (spriteB != NULL&&spriteA != NULL &&spriteA->getTag() == GameSceneNodeBatchTagEnemyBullet &&spriteB->getTag() == GameSceneNodeBatchTagBullet) {
			explodeBullet((Bullet *)spriteA);
			removeChild(spriteB);
			spriteA->setVisible(false);
			if (!en1->bullet->isVisible())
				en1->bullet = NULL;
			else if (!en2->bullet->isVisible())
				en2->bullet = NULL;
			else if (!en3->bullet->isVisible())
				en3->bullet = NULL;
			removeChild(spriteA);
			return true;
		}
		///////////////////////////检测 炮弹与炮弹的接触 end//////////////////////////////////
		////////////////////////////检测 自己与道具的接触 start///////////////////////////////
		else if (spriteB != NULL&&spriteA != NULL &&spriteA->getTag() == GameSceneNodeTagTool&&spriteB->getTag() == GameSceneNodeTagFighter) {
			log("gggggggggggggggggggggggggggggggggggggggg");
			scheduleOnce(schedule_selector(GameScene::deleteTool), 0.0f);
			return true;
		}
		else if (spriteB != NULL&&spriteA != NULL &&spriteB->getTag() == GameSceneNodeTagTool&&spriteA->getTag() == GameSceneNodeTagFighter) {
			log("gggggggggggggggggggggggggggggggggggggggg");
			scheduleOnce(schedule_selector(GameScene::deleteTool), 0.0f);
			return true;
		}
		////////////////////////////检测 自己与道具的接触 end/////////////////////////////////
		return false;
	};
	eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);
	listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event * event) {
		log("Key with keycode %d pressed", keyCode);
		float x = _player->getPosition().x;
		float y = _player->getPosition().y;
		if ((int)keyCode == 29) {
			direction = 'd';
			CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tank1_d.png");
			_player->setDisplayFrame(frame);
			k = keyCode;
			schedule(schedule_selector(GameScene::move), 0.04f);
		}
		else if ((int)keyCode == 28) {
			direction = 'u';
			CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tank1_u.png");
			_player->setDisplayFrame(frame);
			k = keyCode;
			schedule(schedule_selector(GameScene::move), 0.04f);
		}
		else if ((int)keyCode == 26) {
			direction = 'l';
			CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tank1_l.png");
			_player->setDisplayFrame(frame);
			k = keyCode;
			schedule(schedule_selector(GameScene::move), 0.04f);
		}
		else if ((int)keyCode == 27) {
			direction = 'r';
			CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tank1_r.png");
			_player->setDisplayFrame(frame);
			k = keyCode;
			schedule(schedule_selector(GameScene::move), 0.04f);
		}
		else if ((int)keyCode == 59) {
			if (this->getChildByTag(GameSceneNodeBatchTagBullet) == NULL) {
				direction_bullet = direction;
				std::string a = "bullet_" + direction_bullet;
				std::string b = a + ".png";
				Bullet* bullet = Bullet::createWithSpriteFrameName(b);
				bullet->setPosition2(direction_bullet, _player);
				this->addChild(bullet, 0, GameSceneNodeBatchTagBullet);
				shoot = true;
			}
			schedule(schedule_selector(GameScene::moveBullet), 0.025f);
		}
	};
	
	listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event * event) {
		log("Key with keycode %d released", keyCode);
		if ((int)keyCode == 29 || (int)keyCode == 28 || (int)keyCode == 26 || (int)keyCode == 27) {
			unschedule(schedule_selector(GameScene::move));
		}
		else if ((int)keyCode == 59) {
			//unschedule(schedule_selector(GameScene::moveBullet));
			shoot = false;
		}
	};
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	schedule(schedule_selector(GameScene::moveEnemy), 0.04f);
}

int GameScene::Ran()
{
	int n = (rand()) % 5;
	return n;
}

void GameScene::moveEnemy(float dt) {
	auto size = Director::getInstance()->getWinSize();
	srand(time(0));
	int rand;
	float x;
	float y;
	int tileGid = 0;
	int tileGid1 = 0;
	int tileGid2 = 0;
	if (en1 != NULL&&en1->isVisible()) {
		x = en1->getPosition().x;
		y = en1->getPosition().y;
		//移动，A*算法
		if (!en1->getHavetool()) {
			rand = Ran();
			if (rand == 0) {
				//向上
				y += _speed - 2;
				en1->direction("u");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en1->getLifes()) + "_u.png");
				en1->setDisplayFrame(frame);
			}
			else if (rand == 1 || rand == 4) {
				//向下
				y -= _speed - 2;
				en1->direction("d");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en1->getLifes()) + "_d.png");
				en1->setDisplayFrame(frame);
			}
			else if (rand == 2) {
				//向左
				x -= _speed - 2;
				en1->direction("l");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en1->getLifes()) + "_l.png");
				en1->setDisplayFrame(frame);
			}
			else {
				//向右
				x += _speed - 2;
				en1->direction("r");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en1->getLifes()) + "_r.png");
				en1->setDisplayFrame(frame);
			}
		}
		else {
			if (sssss < path->count() && path->count()>1) {
				x = ((AstarItem *)(path->getObjectAtIndex(sssss)))->getcol();
				y = ((AstarItem *)(path->getObjectAtIndex(sssss)))->getrow();
				if (x - en1->getPosition().x > 0) {//向右
					en1->direction("r");
					CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_r.png");
					en1->setDisplayFrame(frame);
				}
				else if (x - en1->getPosition().x < 0) {//向左
					en1->direction("l");
					CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_l.png");
					en1->setDisplayFrame(frame);
				}
				else if (y - en1->getPosition().y > 0) {//向上
					en1->direction("u");
					CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_u.png");
					en1->setDisplayFrame(frame);
				}
				else if (y - en1->getPosition().y < 0) {//向下
					en1->direction("d");
					CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_d.png");
					en1->setDisplayFrame(frame);
				}
			}
			else if (sssss == path->count()) {
				en1->direction("r");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_r.png");
				en1->setDisplayFrame(frame);
			}
		}
		if (en1->bullet == NULL) {
			en1->isBullet();
			addChild(en1->bullet, 0, GameSceneNodeBatchTagEnemyBullet);
		}
		else {
			if (en1 != NULL)
				enemyBullet(en1);
		}
		if (x <= 0 || y <= 0 || x + en1->getContentSize().width >= tileMap->getContentSize().width || y + en1->getContentSize().height >= size.height);
		else if (_player->checkcrash(x, y, en1->getContentSize().width, en1->getContentSize().height));
		else if (en3 != NULL&&en3->checkcrash(x, y, en1->getContentSize().width, en1->getContentSize().height));
		else if (en2 != NULL&&en2->checkcrash(x, y, en1->getContentSize().width, en1->getContentSize().height));
		else {
			for (int i = x; i <= x + en1->getContentSize().width; i++) {
				for (int j = y; j <= y + en1->getContentSize().height; j++) {
					if (!ironbase->isVisible()) {
						if (background->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)))>0)
							if (background->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
						if (base->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)))>0)
							if (base->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
					}
					else {
						if (ironbase->getTileGIDAt(this->tileCoordFromPosition(Point(i, j))) > 0)
							if (ironbase->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
						if (background->getTileGIDAt(this->tileCoordFromPosition(Point(i, j))) > 0)
							if (background->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
					}
					//tileGid1 = basecoll->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)));
					//tileGid2 = ironbasecoll->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)));
					/*if (tileGid > 0) {
						break;
					}*/
				}
				if (tileGid > 0)
					break;
			}
			if (tileGid == 0) {
				en1->setPosition(x, y);
				if (en1->getHavetool()) {
					sssss++;
				}
				else
					sssss = 0;
			}
		}
	}
	tileGid = 0;
	if (en2 != NULL&&en2->isVisible()) {
		x = en2->getPosition().x;
		y = en2->getPosition().y;
		rand = Ran();
		if (!en2->getHavetool()) {
			if (rand == 0) {
				//向上
				y += _speed - 2;
				en2->direction("u");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en2->getLifes()) + "_u.png");
				en2->setDisplayFrame(frame);
			}
			else if (rand == 1 || rand == 4) {
				//向下
				y -= _speed - 2;
				en2->direction("d");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en2->getLifes()) + "_d.png");
				en2->setDisplayFrame(frame);
			}
			else if (rand == 2) {
				//向左
				x -= _speed - 2;
				en2->direction("l");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en2->getLifes()) + "_l.png");
				en2->setDisplayFrame(frame);
			}
			else {
				//向右
				x += _speed - 2;
				en2->direction("r");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en2->getLifes()) + "_r.png");
				en2->setDisplayFrame(frame);
			}
		}
		else {
			if (rand == 0) {
				//向上
				y += _speed - 2;
				en2->direction("u");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_u.png");
				en2->setDisplayFrame(frame);
			}
			else if (rand == 1 || rand == 4) {
				//向下
				y -= _speed - 2;
				en2->direction("d");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_d.png");
				en2->setDisplayFrame(frame);
			}
			else if (rand == 2) {
				//向左
				x -= _speed - 2;
				en2->direction("l");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_l.png");
				en2->setDisplayFrame(frame);
			}
			else {
				//向右
				x += _speed - 2;
				en2->direction("r");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_r.png");
				en2->setDisplayFrame(frame);
			}
		}
		if (en2->bullet == NULL) {
			en2->isBullet();
			addChild(en2->bullet, 0, GameSceneNodeBatchTagEnemyBullet);
		}
		else {
			if (en2 != NULL)
				enemyBullet(en2);
		}
		if (x <= 0 || y <= 0 || x + en2->getContentSize().width >= tileMap->getContentSize().width || y + en2->getContentSize().height >= size.height);
		else if (_player->checkcrash(x, y, en2->getContentSize().width, en2->getContentSize().height));
		else if (en1 != NULL&&en1->checkcrash(x, y, en2->getContentSize().width, en2->getContentSize().height));
		else if (en3 != NULL&&en3->checkcrash(x, y, en2->getContentSize().width, en2->getContentSize().height));
		else {
			for (int i = x; i <= x + en2->getContentSize().width; i++) {
				for (int j = y; j <= y + en2->getContentSize().height; j++) {
					if (!ironbase->isVisible()) {
						if (background->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)))>0)
							if (background->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
						if (base->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)))>0)
							if (base->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
					}
					else {
						if (ironbase->getTileGIDAt(this->tileCoordFromPosition(Point(i, j))) > 0)
							if (ironbase->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
						if (background->getTileGIDAt(this->tileCoordFromPosition(Point(i, j))) > 0)
							if (background->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
					}
					//tileGid1 = basecoll->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)));
					//tileGid2 = ironbasecoll->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)));
					/*if (tileGid > 0) {
					break;
					}*/
				}
				if (tileGid > 0)
					break;
			}
			if (tileGid == 0 ) {
				en2->setPosition(x, y);
			}
		}
	}
	tileGid = 0;
	if (en3 != NULL&&en3->isVisible()) {
		x = en3->getPosition().x;
		y = en3->getPosition().y;
		rand = Ran();
		if (!en3->getHavetool()) {
			if (rand == 0) {
				//向上
				y += _speed - 2;
				en3->direction("u");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en3->getLifes()) + "_u.png");
				en3->setDisplayFrame(frame);
			}
			else if (rand == 1 || rand == 4) {
				//向下
				y -= _speed - 2;
				en3->direction("d");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en3->getLifes()) + "_d.png");
				en3->setDisplayFrame(frame);
			}
			else if (rand == 2) {
				//向左
				x -= _speed - 2;
				en3->direction("l");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en3->getLifes()) + "_l.png");
				en3->setDisplayFrame(frame);
			}
			else {
				//向右
				x += _speed - 2;
				en3->direction("r");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy" + std::to_string(en3->getLifes()) + "_r.png");
				en3->setDisplayFrame(frame);
			}
		}
		else {
			if (rand == 0) {
				//向上
				y += _speed - 2;
				en3->direction("u");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_u.png");
				en3->setDisplayFrame(frame);
			}
			else if (rand == 1 || rand == 4) {
				//向下
				y -= _speed - 2;
				en3->direction("d");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_d.png");
				en3->setDisplayFrame(frame);
			}
			else if (rand == 2) {
				//向左
				x -= _speed - 2;
				en3->direction("l");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_l.png");
				en3->setDisplayFrame(frame);
			}
			else {
				//向右
				x += _speed - 2;
				en3->direction("r");
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemyT_r.png");
				en3->setDisplayFrame(frame);
			}
		}
		if (en3->bullet == NULL) {
			en3->isBullet();
			addChild(en3->bullet, 0, GameSceneNodeBatchTagEnemyBullet);
		}
		else {
			if (en3!= NULL)
				enemyBullet(en3);
		}
		if (x <= 0 || y <= 0 || x + en3->getContentSize().width >= tileMap->getContentSize().width || y + en3->getContentSize().height >= size.height);
		else if (_player->checkcrash(x, y, en3->getContentSize().width, en3->getContentSize().height));
		else if (en1 != NULL&&en1->checkcrash(x, y, en3->getContentSize().width, en3->getContentSize().height));
		else if (en2 != NULL&&en2->checkcrash(x, y, en3->getContentSize().width, en3->getContentSize().height));
		else {
			for (int i = x; i <= x + en3->getContentSize().width; i++) {
				for (int j = y; j <= y + en3->getContentSize().height; j++) {
					if (!ironbase->isVisible()) {
						if (background->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)))>0)
							if (background->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
						if (base->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)))>0)
							if (base->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
					}
					else {
						if (ironbase->getTileGIDAt(this->tileCoordFromPosition(Point(i, j))) > 0)
							if (ironbase->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
						if (background->getTileGIDAt(this->tileCoordFromPosition(Point(i, j))) > 0)
							if (background->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible()) {
								tileGid = 1;
								break;
							}
					}
					//tileGid1 = basecoll->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)));
					//tileGid2 = ironbasecoll->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)));
					/*if (tileGid > 0) {
					break;
					}*/
				}
				if (tileGid > 0)
					break;
			}
			if (tileGid == 0) 
				en3->setPosition(x, y);
		}
	}
}

void GameScene::move(float dt) {
	log("moving!!!!!!!!!");
	auto size = Director::getInstance()->getWinSize();
	float x = _player->getPosition().x;
	float y = _player->getPosition().y;
	if ((int)k == 29&&down == true) {
		direction_move = "d";
		y -= _speed;
	}
	else if ((int)k == 28&&up == true) {
		direction_move = "u";
		y += _speed;
	}
	else if ((int)k == 26&&left == true) {
		direction_move = 'l';
		x -= _speed;
	}
	else if ((int)k == 27&&right == true) {
		direction_move = 'r';
		x += _speed;
	}
	if (x <= 0 || y <= 0 || x+ _player->getContentSize().width >= tileMap->getContentSize().width || y + _player->getContentSize().height >= size.height)
		return;
	int tileGid = 0, tileGid1 = 0, tileGid2 = 0;
	this->getChildByTag(GameSceneNodeBatchTagEnemy);
	if (en1 != NULL){
		log("1");
		if (en1->checkcrash(x, y, _player->getContentSize().width, _player->getContentSize().height)) {
			return;
		}
	}
	if (en2 != NULL) {
		log("2");
		if (en2->checkcrash(x, y, _player->getContentSize().width, _player->getContentSize().height)) {
			return;
		}
	}
	if (en3 != NULL) {
		log("3");
		if (en3->checkcrash(x, y, _player->getContentSize().width, _player->getContentSize().height)) {
			return;
		}
	}
	if (tool!= NULL) {
		if (tool->checkcrash(x, y, _player->getContentSize().width, _player->getContentSize().height)) {
			scheduleOnce(schedule_selector(GameScene::deleteTool), 0.0f);
		}
	}
	for (int i = x; i <= x + _player->getContentSize().width; i++) {
		for (int j = y; j <= y + _player->getContentSize().height; j++) {
			if (!ironbase->isVisible()) {
				if(background->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)))>0)
					if (background->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible())
						return;
				if (base->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)))>0)
					if (base->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible())
						return;
			}
			else {
				if (ironbase->getTileGIDAt(this->tileCoordFromPosition(Point(i, j))) > 0)
					if (ironbase->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible())
						return;
				if (background->getTileGIDAt(this->tileCoordFromPosition(Point(i, j))) > 0)
					if (background->getTileAt(this->tileCoordFromPosition(Point(i, j)))->isVisible())
						return;
			}
			//tileGid1 = basecoll->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)));
			//tileGid2 = ironbasecoll->getTileGIDAt(this->tileCoordFromPosition(Point(i, j)));
			/*if (tileGid > 0)
				return;*/
		}
	}
	_player->setPosition(Vec2(x, y));
}

//实现从像素点坐标转化为瓦片坐标
Point GameScene::tileCoordFromPosition(Point pos)
{
	int x = pos.x / tileMap->getTileSize().width;
	int y = ((tileMap->getMapSize().height * tileMap->getTileSize().height) - pos.y) /tileMap->getTileSize().height;         
	return Point(x, y);
}

/*
void GameScene::movedown(float dt) {
	log("movingdown!!!!!!!!!");
	float x = _player->getPosition().x;
	float y = _player->getPosition().y;
	y -= _speed;
	_player->setPosition(Vec2(x, y));
}

void GameScene::moveup(float dt) {
	log("movingup!!!!!!!!!");
	float x = _player->getPosition().x;
	float y = _player->getPosition().y;
	y += _speed;
	_player->setPosition(Vec2(x, y));
}

void GameScene::moveleft(float dt) {
	log("movingleft!!!!!!!!!");
	float x = _player->getPosition().x;
	float y = _player->getPosition().y;
	x -= _speed;
	_player->setPosition(Vec2(x, y));
}

void GameScene::moveright(float dt) {
	log("movingright!!!!!!!!!");
	float x = _player->getPosition().x;
	float y = _player->getPosition().y;
	x += _speed;
	_player->setPosition(Vec2(x, y));
}*/

//坦克发射炮弹
/*
void GameScene::shootBullet_Enemy(float dt)
{
	if (_player && _player->isVisible()) {
		Bullet* bullet = Bullet::createWithSpriteFrameName("bullet.png");
		bullet->setVelocity(Vec2(0, GameSceneBulletVelocity));
		this->addChild(bullet, 0, GameSceneNodeBatchTagBullet);
		bullet->shootBulletFromFighter(_player);

		float x = bullet->getPosition().x;
		float y = bullet->getPosition().y;
		Point tileCoord1 = this->tileCoordFromPosition(Point(x, y));
		Point tileCoord2 = this->tileCoordFromPosition(Point(x + bullet->getContentSize().width, y));
		Point tileCoord3 = this->tileCoordFromPosition(Point(x, y + bullet->getContentSize().height));
		Point tileCoord4 = this->tileCoordFromPosition(Point(x + bullet->getContentSize().width, y + bullet->getContentSize().height));
		//获得瓦片的GID  
		int tileGid1 = collidable->getTileGIDAt(tileCoord1);
		int tileGid2 = collidable->getTileGIDAt(tileCoord2);
		int tileGid3 = collidable->getTileGIDAt(tileCoord3);
		int tileGid4 = collidable->getTileGIDAt(tileCoord4);
		if (tileGid1 > 0 ) {
			log("aaaaaaaaa");
			TMXLayer* layer = tileMap->getLayer("bg1");
			layer->removeTileAt(tileCoord1);
			collidable->removeTileAt(tileCoord1);
		}
		if (tileGid2 > 0) {
			log("aaaaaaaaa");
			TMXLayer* layer = tileMap->getLayer("bg1");
			layer->removeTileAt(tileCoord2);
			collidable->removeTileAt(tileCoord2);
		}
		if (tileGid3 > 0) {
			log("aaaaaaaaa");
			TMXLayer* layer = tileMap->getLayer("bg1");
			layer->removeTileAt(tileCoord3);
			collidable->removeTileAt(tileCoord3);
		}
		if (tileGid4 > 0) {
			log("aaaaaaaaa");
			TMXLayer* layer = tileMap->getLayer("bg1");
			layer->removeTileAt(tileCoord4);
			collidable->removeTileAt(tileCoord4);
		}
		scheduleOnce(schedule_selector(GameScene::turnshoot), 0.6f);
	}
}
*/

void GameScene::shootBullet(float dt){
	scheduleOnce(schedule_selector(GameScene::moveBullet), 0.025f);
}

void GameScene::moveBullet(float dt) {
	Bullet* sp = (Bullet*)this->getChildByTag(GameSceneNodeBatchTagBullet); //获取 tag=100 的精灵
	if (sp == NULL&&shoot == true) {
		direction_bullet = direction;
		std::string a = "bullet_" + direction_bullet;
		std::string b = a + ".png";
		Bullet* bullet = Bullet::createWithSpriteFrameName(b);
		bullet->setPosition2(direction_bullet, _player);
		this->addChild(bullet, 0, GameSceneNodeBatchTagBullet);
	}
	else if(sp!=NULL){
		sp->move(direction_bullet);
		float x = sp->getPosition().x;
		float y = sp->getPosition().y;
		if (x <= 0 || y <= 0 || x + _player->getContentSize().width >= tileMap->getContentSize().width || y + _player->getContentSize().height >= Director::getInstance()->getWinSize().height);
		else {
			Point tileCoord1 = this->tileCoordFromPosition(Point(x, y));
			Point tileCoord2 = this->tileCoordFromPosition(Point(x + sp->getContentSize().width, y));
			Point tileCoord3 = this->tileCoordFromPosition(Point(x, y + sp->getContentSize().height));
			Point tileCoord4 = this->tileCoordFromPosition(Point(x + sp->getContentSize().width, y + sp->getContentSize().height));
			//获得瓦片的GID  
			if (!ironbase->isVisible()) {
				int tileGid1 = background->getTileGIDAt(tileCoord1);
				int tileGid2 = background->getTileGIDAt(tileCoord2);
				int tileGid3 = background->getTileGIDAt(tileCoord3);
				int tileGid4 = background->getTileGIDAt(tileCoord4);
				int tileGid5 = base->getTileGIDAt((tileCoord1));
				int tileGid6 = base->getTileGIDAt((tileCoord2));
				int tileGid7 = base->getTileGIDAt((tileCoord3));
				int tileGid8 = base->getTileGIDAt((tileCoord4));
				if (tileGid1 > 0 && background->getTileAt(tileCoord1)->isVisible() && sp != NULL) {//如果背景该位置有像素
					log("aaaaaaaaa");
					//background->removeTileAt(tileCoord1);
					//collidable->removeTileAt(tileCoord1);
					background->getTileAt(tileCoord1)->setVisible(false);
					//collidable->getTileAt(tileCoord1)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
				else if (tileGid2 > 0 && background->getTileAt(tileCoord2)->isVisible() && sp != NULL) {
					log("aaaaaaaaa");
					background->getTileAt(tileCoord2)->setVisible(false);
					//collidable->removeTileAt(tileCoord2);
					//collidable->getTileAt(tileCoord2)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
				else if (tileGid3 > 0 && background->getTileAt(tileCoord3)->isVisible() && sp != NULL) {
					log("aaaaaaaaa");
					//background->removeTileAt(tileCoord3);
					//collidable->removeTileAt(tileCoord3);
					background->getTileAt(tileCoord3)->setVisible(false);
					//collidable->getTileAt(tileCoord3)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
				else if (tileGid4 > 0 && background->getTileAt(tileCoord4)->isVisible() && sp != NULL) {
					log("aaaaaaaaa");
					//background->removeTileAt(tileCoord4);
					//collidable->removeTileAt(tileCoord4);
					background->getTileAt(tileCoord4)->setVisible(false);
					//collidable->getTileAt(tileCoord4)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
				else if (tileGid5 > 0 && base->getTileAt(tileCoord1)->isVisible() && sp != NULL) {
					log("aaaaaaaaa");
					//base->removeTileAt(tileCoord1);
					//basecoll->removeTileAt(tileCoord1);
					base->getTileAt(tileCoord1)->setVisible(false);
					//basecoll->getTileAt(tileCoord1)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
				else if (tileGid6 > 0 && base->getTileAt(tileCoord2)->isVisible() && sp != NULL) {
					log("aaaaaaaaa");
					//base->removeTileAt(tileCoord2);
					//basecoll->removeTileAt(tileCoord2);
					base->getTileAt(tileCoord2)->setVisible(false);
					//basecoll->getTileAt(tileCoord2)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
				else if (tileGid7 > 0 && base->getTileAt(tileCoord3)->isVisible() && sp != NULL) {
					log("aaaaaaaaa");
					//base->removeTileAt(tileCoord3);
					//basecoll->removeTileAt(tileCoord3);
					base->getTileAt(tileCoord3)->setVisible(false);
				//	basecoll->getTileAt(tileCoord3)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
				else if (tileGid8 > 0 && base->getTileAt(tileCoord4)->isVisible() && sp != NULL) {
					log("aaaaaaaaa");
					//base->removeTileAt(tileCoord4);
					//basecoll->removeTileAt(tileCoord4);
					base->getTileAt(tileCoord4)->setVisible(false);
					//basecoll->getTileAt(tileCoord4)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
			}
			else {
				int tileGid1 = background->getTileGIDAt(tileCoord1);
				int tileGid2 = background->getTileGIDAt(tileCoord2);
				int tileGid3 = background->getTileGIDAt(tileCoord3);
				int tileGid4 = background->getTileGIDAt(tileCoord4);
				int tileGid5 = ironbase->getTileGIDAt((tileCoord1));
				int tileGid6 = ironbase->getTileGIDAt((tileCoord2));
				int tileGid7 = ironbase->getTileGIDAt((tileCoord3));
				int tileGid8 = ironbase->getTileGIDAt((tileCoord4));
				if (tileGid5 > 0 || tileGid6 > 0 || tileGid7 > 0 || tileGid8 > 0) {
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
				if (tileGid1 > 0 && background->getTileAt(tileCoord1)->isVisible() && sp != NULL) {//如果背景该位置有像素
					log("aaaaaaaaa");
					//background->removeTileAt(tileCoord1);
					//collidable->removeTileAt(tileCoord1);
					background->getTileAt(tileCoord1)->setVisible(false);
					//collidable->getTileAt(tileCoord1)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
				else if (tileGid2 > 0 && background->getTileAt(tileCoord2)->isVisible() && sp != NULL) {
					log("aaaaaaaaa");
					background->getTileAt(tileCoord2)->setVisible(false);
					//collidable->removeTileAt(tileCoord2);
					//collidable->getTileAt(tileCoord2)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
				else if (tileGid3 > 0 && background->getTileAt(tileCoord3)->isVisible() && sp != NULL) {
					log("aaaaaaaaa");
					//background->removeTileAt(tileCoord3);
					//collidable->removeTileAt(tileCoord3);
					background->getTileAt(tileCoord3)->setVisible(false);
					//collidable->getTileAt(tileCoord3)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
				else if (tileGid4 > 0 && background->getTileAt(tileCoord4)->isVisible() && sp != NULL) {
					log("aaaaaaaaa");
					//background->removeTileAt(tileCoord4);
					//collidable->removeTileAt(tileCoord4);
					background->getTileAt(tileCoord4)->setVisible(false);
					//collidable->getTileAt(tileCoord4)->setVisible(false);
					removeChildByTag(GameSceneNodeBatchTagBullet);
					return;
				}
			}
		}
		if (sp->getPosition().y > Director::getInstance()->getWinSize().height|| sp->getPosition().x<0||sp->getPosition().y<0||sp->getPosition().x>tileMap->getContentSize().width) {
			removeChildByTag(GameSceneNodeBatchTagBullet);
		}
	}
}

void GameScene::turnshoot(float dt) {
	//shoot = true;
}

void GameScene::enemyBullet(Tank *a) {
	Bullet *sp = a->bullet;
	sp->move(a->_bullet_direction);
	float x = sp->getPosition().x;
	float y = sp->getPosition().y;
	if (x <= 0 || y <= 0 || x + _player->getContentSize().width >= tileMap->getContentSize().width || y + _player->getContentSize().height >= Director::getInstance()->getWinSize().height);
	else {
		Point tileCoord1 = this->tileCoordFromPosition(Point(x, y));
		Point tileCoord2 = this->tileCoordFromPosition(Point(x + sp->getContentSize().width, y));
		Point tileCoord3 = this->tileCoordFromPosition(Point(x, y + sp->getContentSize().height));
		Point tileCoord4 = this->tileCoordFromPosition(Point(x + sp->getContentSize().width, y + sp->getContentSize().height));
		//获得瓦片的GID  
		if (!ironbase->isVisible()) {
			int tileGid1 = background->getTileGIDAt(tileCoord1);
			int tileGid2 = background->getTileGIDAt(tileCoord2);
			int tileGid3 = background->getTileGIDAt(tileCoord3);
			int tileGid4 = background->getTileGIDAt(tileCoord4);
			int tileGid5 = base->getTileGIDAt((tileCoord1));
			int tileGid6 = base->getTileGIDAt((tileCoord2));
			int tileGid7 = base->getTileGIDAt((tileCoord3));
			int tileGid8 = base->getTileGIDAt((tileCoord4));
			if (tileGid1 > 0 && background->getTileAt(tileCoord1)->isVisible() && sp != NULL) {//如果背景该位置有像素
				log("aaaaaaaaa");
				//background->removeTileAt(tileCoord1);
				//collidable->removeTileAt(tileCoord1);
				background->getTileAt(tileCoord1)->setVisible(false);
				//collidable->getTileAt(tileCoord1)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
			else if (tileGid2 > 0 && background->getTileAt(tileCoord2)->isVisible() && sp != NULL) {
				log("aaaaaaaaa");
				background->getTileAt(tileCoord2)->setVisible(false);
				//collidable->removeTileAt(tileCoord2);
				//collidable->getTileAt(tileCoord2)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
			else if (tileGid3 > 0 && background->getTileAt(tileCoord3)->isVisible() && sp != NULL) {
				log("aaaaaaaaa");
				//background->removeTileAt(tileCoord3);
				//collidable->removeTileAt(tileCoord3);
				background->getTileAt(tileCoord3)->setVisible(false);
				//collidable->getTileAt(tileCoord3)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
			else if (tileGid4 > 0 && background->getTileAt(tileCoord4)->isVisible() && sp != NULL) {
				log("aaaaaaaaa");
				//background->removeTileAt(tileCoord4);
				//collidable->removeTileAt(tileCoord4);
				background->getTileAt(tileCoord4)->setVisible(false);
				//collidable->getTileAt(tileCoord4)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
			else if (tileGid5 > 0 && base->getTileAt(tileCoord1)->isVisible() && sp != NULL) {
				log("aaaaaaaaa");
				//base->removeTileAt(tileCoord1);
				//basecoll->removeTileAt(tileCoord1);
				base->getTileAt(tileCoord1)->setVisible(false);
				//basecoll->getTileAt(tileCoord1)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
			else if (tileGid6 > 0 && base->getTileAt(tileCoord2)->isVisible() && sp != NULL) {
				log("aaaaaaaaa");
				//base->removeTileAt(tileCoord2);
				//basecoll->removeTileAt(tileCoord2);
				base->getTileAt(tileCoord2)->setVisible(false);
				//basecoll->getTileAt(tileCoord2)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
			else if (tileGid7 > 0 && base->getTileAt(tileCoord3)->isVisible() && sp != NULL) {
				log("aaaaaaaaa");
				//base->removeTileAt(tileCoord3);
				//basecoll->removeTileAt(tileCoord3);
				base->getTileAt(tileCoord3)->setVisible(false);
				//	basecoll->getTileAt(tileCoord3)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
			else if (tileGid8 > 0 && base->getTileAt(tileCoord4)->isVisible() && sp != NULL) {
				log("aaaaaaaaa");
				//base->removeTileAt(tileCoord4);
				//basecoll->removeTileAt(tileCoord4);
				base->getTileAt(tileCoord4)->setVisible(false);
				//basecoll->getTileAt(tileCoord4)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
		}
		else {
			int tileGid1 = background->getTileGIDAt(tileCoord1);
			int tileGid2 = background->getTileGIDAt(tileCoord2);
			int tileGid3 = background->getTileGIDAt(tileCoord3);
			int tileGid4 = background->getTileGIDAt(tileCoord4);
			int tileGid5 = ironbase->getTileGIDAt((tileCoord1));
			int tileGid6 = ironbase->getTileGIDAt((tileCoord2));
			int tileGid7 = ironbase->getTileGIDAt((tileCoord3));
			int tileGid8 = ironbase->getTileGIDAt((tileCoord4));
			if (tileGid5 > 0 || tileGid6 > 0 || tileGid7 > 0 || tileGid8 > 0) {
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
			if (tileGid1 > 0 && background->getTileAt(tileCoord1)->isVisible() && sp != NULL) {//如果背景该位置有像素
				log("aaaaaaaaa");
				//background->removeTileAt(tileCoord1);
				//collidable->removeTileAt(tileCoord1);
				background->getTileAt(tileCoord1)->setVisible(false);
				//collidable->getTileAt(tileCoord1)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
			else if (tileGid2 > 0 && background->getTileAt(tileCoord2)->isVisible() && sp != NULL) {
				log("aaaaaaaaa");
				background->getTileAt(tileCoord2)->setVisible(false);
				//collidable->removeTileAt(tileCoord2);
				//collidable->getTileAt(tileCoord2)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
			else if (tileGid3 > 0 && background->getTileAt(tileCoord3)->isVisible() && sp != NULL) {
				log("aaaaaaaaa");
				//background->removeTileAt(tileCoord3);
				//collidable->removeTileAt(tileCoord3);
				background->getTileAt(tileCoord3)->setVisible(false);
				//collidable->getTileAt(tileCoord3)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
			else if (tileGid4 > 0 && background->getTileAt(tileCoord4)->isVisible() && sp != NULL) {
				log("aaaaaaaaa");
				//background->removeTileAt(tileCoord4);
				//collidable->removeTileAt(tileCoord4);
				background->getTileAt(tileCoord4)->setVisible(false);
				//collidable->getTileAt(tileCoord4)->setVisible(false);
				a->_bullet_direction = a->_direction;
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
				sp->setDisplayFrame(frame);
				sp->setPosition2(a->_bullet_direction, a);
				return;
			}
		}
	}
	if (sp->getPosition().y > Director::getInstance()->getWinSize().height || sp->getPosition().x<0 || sp->getPosition().y<0 || sp->getPosition().x>tileMap->getContentSize().width) {
		a->_bullet_direction = a->_direction;
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet_" + a->_bullet_direction + ".png");
		sp->setDisplayFrame(frame);
		sp->setPosition2(a->_bullet_direction, a);
	}
}

void GameScene::explodeBullet(Bullet *bullet) {
	auto sprite = Sprite::create();
	sprite->setAnchorPoint(ccp(0.5, 0.5));
	sprite->setPosition(bullet->getPosition());
	this->addChild(sprite);
	//创建精灵帧CCSpriteFrame数组
	Vector<SpriteFrame*> framelist;
	for (int i = 3; i >= 1; i--) {
		char str[50];
		sprintf(str, "explode-%d.png", i);
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		framelist.pushBack(frame);
	}
	//使用精灵帧数组创建，单位帧间隔为2.0/14.0秒
	Animation* animation = Animation::createWithSpriteFrames(framelist, 0.1);
	CCAnimate* animate = CCAnimate::create(animation);
	//回调函数  
	auto repeat = Repeat::create(animate, 1);
	auto callback = CallFunc::create(CC_CALLBACK_0(GameScene::bombDone, this, sprite));
	auto sequence = Sequence::create(repeat, callback, NULL);
	sprite->runAction(sequence);
}

void GameScene::bombDone(Node* sender)
{
	sender->removeFromParentAndCleanup(true);
}

void GameScene::showTank(Tank *tank) {
	tank->setVisible(false);
	auto sprite = Sprite::create();
	sprite->setPosition(tank->getPosition());
	sprite->setAnchorPoint(ccp(0, 0));
	this->addChild(sprite);
	//创建精灵帧CCSpriteFrame数组
	Vector<SpriteFrame*> framelist;
	for (int i = 1; i <=2; i++) {
		char str[50];
		sprintf(str, "xing%d.png", i);
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		framelist.pushBack(frame);
	}
	//使用精灵帧数组创建，单位帧间隔为2.0/14.0秒
	Animation* animation = Animation::createWithSpriteFrames(framelist, 1.5);
	CCAnimate* animate = CCAnimate::create(animation);
	//回调函数  
	auto repeat = Repeat::create(animate, 1);
	auto callback = CallFunc::create(CC_CALLBACK_0(GameScene::showDone, this, sprite ,tank));
	auto sequence = Sequence::create(repeat, callback, NULL);
	sprite->runAction(sequence);
}

void GameScene::showDone(Node* sender,Tank *tank)
{
	sender->removeFromParentAndCleanup(true);
	tank->setVisible(true);
}

void GameScene::showTool(std::string a) {
	srand(time(0));
	int position_num = rand() % 10;
	position_num++;
	group = tileMap->getObjectGroup("objects");
	ValueMap spawnPoint = group->getObject("t" + std::to_string(position_num));
	float x = spawnPoint["x"].asFloat();
	float y = spawnPoint["y"].asFloat();
	tool = Tool::createWithSpriteFrameName(a.c_str());
	tool->setAnchorPoint(ccp(0, 0));
	tool->setPosition(Vec2(x, y));
	tool->num = 1;
	this->addChild(tool, 0, GameSceneNodeTagTool);
	//scheduleOnce(schedule_selector(GameScene::deleteTool), 5.0f);
}

void GameScene::deleteTool(float dt) {
	if (tool->num == 1) {
		ironbase->setVisible(true);
		schedule(schedule_selector(GameScene::resetBase), 10.0f);
	}
	removeChildByTag(GameSceneNodeTagTool);
	tool = NULL;
}

void GameScene::resetBase(float dt) {
	ironbase->setVisible(false);
	/*auto layersize = base->getLayerSize();
	auto tiles = base->getTiles();
	auto tilesEnd = tiles + static_cast<uint32_t>(layersize.width*layersize.height);
	for (; tiles < tilesEnd; ++tiles) {
		if (*tiles != 0) {*/
	base->setVisible(true);
}