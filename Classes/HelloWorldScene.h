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
	int colorOrder = 0;
	int orderNumber = 1;
	int number[641] = { 0 };
	DynamicScheduler *ds;
	Color4F colors[7] = { Color4F::BLACK,Color4F::BLUE,Color4F::RED,Color4F::YELLOW,Color4F::GRAY,Color4F::GREEN,Color4F::ORANGE };
public:
    static DynamicScene* createScene(int method);
    virtual bool init();
	void enemyInit(int method);
	void addBlock();
	void addProcess();
    void menuCloseCallback(Ref* pSender);
	void nextCallback(Ref *pSender);
	void addEndButton();
    CREATE_FUNC(DynamicScene);
};

#endif // __HELLOWORLD_SCENE_H__
