#include "cocos2d.h"

//�̳�Layerͼ����
class Welcome : public cocos2d::Layer {
public:
	//��������
	static cocos2d::Scene* createScene();
	//��ʼ��
	virtual bool init();
	//����
	void start();
	//����
	void construction();
	//��߷�
	void highscores();
	//����
	void set();
	//����
	void help();
	//����ͼ��Layer�ĺ�
	CREATE_FUNC(Welcome);
private:
};
