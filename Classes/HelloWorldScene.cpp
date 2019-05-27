#include "HelloWorldScene.h"
#include "PageScene.h"
#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"

DynamicScene* DynamicScene::createScene(int method)
{
	DynamicScene* enemy = new DynamicScene();
	if (enemy && enemy->init())
	{
		enemy->autorelease();
		enemy->enemyInit(method);
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return NULL;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool DynamicScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    return true;
}

void DynamicScene::enemyInit(int method)
{
	ds = new DynamicScheduler(method);//初始化调度器    
	//添加白色背景板和所使用的分区方法名标题
	auto backgroud = LayerColor::create(Color4B::WHITE);
	this->addChild(backgroud);
	auto title = Label::createWithSystemFont((method == DynamicScheduler::FIRST) ? "Fisrt Adaptation" : "Optimal Adaptation", "Arial", 30);
	title->setColor(Color3B::BLACK);
	title->setPosition(480, 750);
	this->addChild(title);

	addEndButton();
	//添加640k的内存块
	auto memory = DrawNode::create();
	memory->drawRect(Vec2(800, 70), Vec2(1000, 710), Color4F::BLACK);
	this->addChild(memory);
	//添加内存块和指令
	addBlock();
	addProcess();
	//添加下一个按钮
	auto next = ui::Button::create("next.png");
	next->setPosition(Vec2(510 - next->getContentSize().width / 2, 425 - next->getContentSize().height / 2));
	next->addClickEventListener(CC_CALLBACK_1(DynamicScene::nextCallback, this));
	this->addChild(next);
	//下一个按钮提示语
	auto label = Label::createWithSystemFont("Next" , "Arial", 30);
	label->setColor(Color3B::BLACK);
	label->setPosition(475, 335);
	this->addChild(label);
}

void DynamicScene::addBlock() {
	//首先判断指令是否已经全部执行完
	if (!ds->processList.empty()) {
		//只有申请内存空间的指令才会添加内存块
		if (ds->processList.front()->order == DynamicScheduler::APPLY) {
			int id = ds->processList.front()->id;
			int size = ds->processList.front()->size;
			//根据内存块所需要的空间大小画出内存块
			auto block = DrawNode::create();
			block->drawSolidRect(Vec2(50, 410 - size / 2), Vec2(250, 410 + size / 2), colors[colorOrder]);
			colorOrder = (colorOrder + 1) % 7;
			block->setTag(id);
			//添加作业号和内存大小id
			auto idNumber = Label::createWithSystemFont(String::createWithFormat("%i", id)->getCString(), "Arial", 25);
			idNumber->setPosition(150, 423);
			idNumber->setColor(Color3B::WHITE);
			block->addChild(idNumber);
			auto sizeNumber= Label::createWithSystemFont(String::createWithFormat("%iK", size)->getCString(), "Arial", 25);
			sizeNumber->setPosition(150, 397);
			sizeNumber->setColor(Color3B::WHITE);
			block->addChild(sizeNumber);
			this->addChild(block);
		}
	}
}

void DynamicScene::addProcess()
{
	//确认指令并没有执行完
	if (!ds->processList.empty()) {
		int id = ds->processList.front()->id;
		int order = ds->processList.front()->order;
		//将下一条指令显示出来
		auto process = DrawNode::create();
		process->drawCircle(Vec2(480, 550), 100, 360, 100, false, Color4F::BLACK);
		//显示指令数，操作作业号和操作请求
		auto number = Label::createWithSystemFont(String::createWithFormat("%i", orderNumber++)->getCString(), "Arial", 25);
		number->setPosition(480, 600);
		number->setColor(Color3B::BLACK);
		auto assignment = Label::createWithSystemFont(String::createWithFormat("assignment %i", id)->getCString(), "Arial", 25);
		assignment->setPosition(480, 550);
		assignment->setColor(Color3B::BLACK);
		auto orderText = Label::createWithSystemFont((order == ds->APPLY) ? "Apply" : "Freed", "Arial", 25);
		orderText->setPosition(480, 500);
		orderText->setColor(Color3B::BLACK);
		process->addChild(number);
		process->addChild(assignment);
		process->addChild(orderText);
		process->setTag(0);
		this->addChild(process);
	}
}


void DynamicScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void DynamicScene::nextCallback(Ref * pSender)
{
	//获取调度器生成的调度信息
	Message *m = ds->allocate();
	if (m == NULL) {
		//当指令全部执行完之后
		//如果当前页面为首次适应算法
		//就跳转到最佳适应算法的界面
		if (ds->getMethod() == DynamicScheduler::FIRST) {
			DynamicScene* scene = DynamicScene::createScene(DynamicScheduler::BEST);
			Director::getInstance()->replaceScene(scene);
		}
		//否则跳转至请求分区调度中的FIFO界面
		//FIFO的指令序列自己随机生成
		else {
			auto scene = PageScene::createScene(PageScheduler::FIFOM, NULL);
			Director::getInstance()->replaceScene(scene);
		}
		return;
	}
	addBlock();
	//移除上一条指令
	DrawNode *n = (DrawNode*)getChildByTag(0);
	n->removeFromParent();
	addProcess();
	//如果当前指令是申请指令，则将准备好的内存块移动到指定的位置
	if (m->order == DynamicScheduler::APPLY) {
		DrawNode *d = (DrawNode*)getChildByTag(m->id);
		MoveTo *move = MoveTo::create(0.5, Vec2(750, 300 - (m->end - m->start) / 2 - m->start));
		d->runAction(move);
		//将对应边界位置数字显示出来
		//只要有任何一个内存块有这个边界就会一直显示这个边界值
		if (number[m->start] == 0) {
			auto startNumber = Label::createWithSystemFont(String::createWithFormat("%iK", m->start)->getCString(), "Arial", 18);
			startNumber->setPosition(750, 710 - m->start);
			startNumber->setColor(Color3B::BLACK);
			startNumber->setTag(100 + m->start);
			this->addChild(startNumber);
		}
		number[m->start]++;
		if (number[m->end] == 0) {
			auto endNumber = Label::createWithSystemFont(String::createWithFormat("%iK", m->end)->getCString(), "Arial", 18);
			endNumber->setPosition(750, 710 - m->end);
			endNumber->setColor(Color3B::BLACK);
			endNumber->setTag(100 + m->end);
			this->addChild(endNumber);
		}
		number[m->end]++;
	}
	else {
		//释放指令就把对应内存块移除
		DrawNode *d = (DrawNode*)getChildByTag(m->id);
		MoveTo *move = MoveTo::create(0.5, Vec2(d->getPosition().x + 500, d->getPosition().y));
		d->runAction(move);
		Label *startNumber = (Label*)getChildByTag(100 + m->start);
		Label *endNumber = (Label*)getChildByTag(100 + m->end);
		//查看是否有边界数值需要删去
		number[m->start]--;
		if (number[m->start] == 0) {
			startNumber->removeFromParent();
		}
		number[m->end]--;
		if (number[m->end] == 0) {
			endNumber->removeFromParent();
		}
	}
}

void DynamicScene::addEndButton()
{
	auto closeItem = MenuItemImage::create("CloseNormal.png",
										   "CloseSelected.png",
										   CC_CALLBACK_1(DynamicScene::menuCloseCallback, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}
