#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "DynamicScheduler.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Scene
{
private:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	int colorOrder = 0;
	int number[641] = { 0 };
	DynamicScheduler *ds = new DynamicScheduler(DynamicScheduler::BEST);
	Color4F colors[7] = { Color4F::BLACK,Color4F::BLUE,Color4F::RED,Color4F::YELLOW,Color4F::GRAY,Color4F::GREEN,Color4F::ORANGE };
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
	void addBlock();
    void menuCloseCallback(Ref* pSender);
	void nextCallback(Ref *pSender);
	void addEndButton();
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
