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
	int method;														//请求分区调度方法
	float speed = 1.0f;												//播放速度
	bool run = false;												//是否正在播放
	bool finish = false;											//是否已经完成
	PageScheduler *ps;												//页面调度器
	Label *hint;													//提示标签
	Label *missingPage;												//缺页数标签
	Label *pageFaultRate;											//缺页率标签
	Label *currentNumber;											//当前已执行指令条数
	Label *currentInstruction;										//当前指令序号
	Label *insLocation;												//指令位置
	Label *speedTime;												//播放速度标签
	Label *physicalAddress;											//物理地址标签
public:
	static PageScene* createScene(int method, int *sequence);		//构造函数以及场景初始化
	virtual bool init();											//为了针对LRU和FIFO进行对比
	void enemyInit(int method, int *sequence);						//将指令序列也作为参数传给下一个场景
	void menuCloseCallback(Ref* pSender);							//关闭按钮回调函数
	void addEndButton();											//添加关闭按钮
	void nextInstruction(float dt);									//下一条指令计时器
	void addPage(Info *inf, int ins);								//添加新页面
	CREATE_FUNC(PageScene);
};

#endif // __PAGESCENE_H__