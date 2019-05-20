#ifndef __PAGESCENE_H__
#define __PAGESCENE_H__

#include "cocos2d.h"
#include "PageScheduler.h"

USING_NS_CC;

class PageScene : public cocos2d::Scene
{
private:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	int method;
	float speed = 1.0f;
	bool run = false;
	bool finish = false;
	PageScheduler *ps;
	Label *hint;
	Label *missingPage;
	Label *pageFaultRate;
	Label *currentNumber;
	Label *currentInstruction;
	Label *insLocation;
	Label *speedTime;
public:
	static PageScene* createScene(int method, int *sequence);
	virtual bool init();
	void enemyInit(int method, int *sequence);
	void menuCloseCallback(Ref* pSender);
	void addEndButton();
	void nextInstruction(float dt);
	void addPage(Info *inf, int ins);
	CREATE_FUNC(PageScene);
};

#endif // __PAGESCENE_H__