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
	int colorOrder = 0;													//当前所使用的颜色
	int orderNumber = 1;												//指令条数
	int number[641] = { 0 };											//记录块与块之间的边界值
	DynamicScheduler *ds;												//内存调度器
	Color4F colors[7] = { Color4F::BLACK,Color4F::BLUE,Color4F::RED,
		Color4F::YELLOW,Color4F::GRAY,Color4F::GREEN,Color4F::ORANGE };	//颜色序列
public:
    static DynamicScene* createScene(int method);						//动态分区场景
    virtual bool init();												//初始化，根据分区
	void enemyInit(int method);											//方式初始化调度器
	void addBlock();													//添加内存块
	void addProcess();													//添加下一条指令显示
    void menuCloseCallback(Ref* pSender);								//关闭按钮回调函数
	void nextCallback(Ref *pSender);									//下一个按钮回调函数	
	void addEndButton();												//添加关闭按钮
    CREATE_FUNC(DynamicScene);
};

#endif // __HELLOWORLD_SCENE_H__
