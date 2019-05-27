#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "DynamicScheduler.h"

USING_NS_CC;

class DynamicScene : public cocos2d::Scene
{
private:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	int colorOrder = 0;													//��ǰ��ʹ�õ���ɫ
	int orderNumber = 1;												//ָ������
	int number[641] = { 0 };											//��¼�����֮��ı߽�ֵ
	DynamicScheduler *ds;												//�ڴ������
	Color4F colors[7] = { Color4F::BLACK,Color4F::BLUE,Color4F::RED,
		Color4F::YELLOW,Color4F::GRAY,Color4F::GREEN,Color4F::ORANGE };	//��ɫ����
public:
    static DynamicScene* createScene(int method);						//��̬��������
    virtual bool init();												//��ʼ�������ݷ���
	void enemyInit(int method);											//��ʽ��ʼ��������
	void addBlock();													//����ڴ��
	void addProcess();													//�����һ��ָ����ʾ
    void menuCloseCallback(Ref* pSender);								//�رհ�ť�ص�����
	void nextCallback(Ref *pSender);									//��һ����ť�ص�����	
	void addEndButton();												//��ӹرհ�ť
    CREATE_FUNC(DynamicScene);
};

#endif // __HELLOWORLD_SCENE_H__
