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
	ds = new DynamicScheduler(method);//��ʼ��������    
	//��Ӱ�ɫ���������ʹ�õķ�������������
	auto backgroud = LayerColor::create(Color4B::WHITE);
	this->addChild(backgroud);
	auto title = Label::createWithSystemFont((method == DynamicScheduler::FIRST) ? "Fisrt Adaptation" : "Optimal Adaptation", "Arial", 30);
	title->setColor(Color3B::BLACK);
	title->setPosition(480, 750);
	this->addChild(title);

	addEndButton();
	//���640k���ڴ��
	auto memory = DrawNode::create();
	memory->drawRect(Vec2(800, 70), Vec2(1000, 710), Color4F::BLACK);
	this->addChild(memory);
	//����ڴ���ָ��
	addBlock();
	addProcess();
	//�����һ����ť
	auto next = ui::Button::create("next.png");
	next->setPosition(Vec2(510 - next->getContentSize().width / 2, 425 - next->getContentSize().height / 2));
	next->addClickEventListener(CC_CALLBACK_1(DynamicScene::nextCallback, this));
	this->addChild(next);
	//��һ����ť��ʾ��
	auto label = Label::createWithSystemFont("Next" , "Arial", 30);
	label->setColor(Color3B::BLACK);
	label->setPosition(475, 335);
	this->addChild(label);
}

void DynamicScene::addBlock() {
	//�����ж�ָ���Ƿ��Ѿ�ȫ��ִ����
	if (!ds->processList.empty()) {
		//ֻ�������ڴ�ռ��ָ��Ż�����ڴ��
		if (ds->processList.front()->order == DynamicScheduler::APPLY) {
			int id = ds->processList.front()->id;
			int size = ds->processList.front()->size;
			//�����ڴ������Ҫ�Ŀռ��С�����ڴ��
			auto block = DrawNode::create();
			block->drawSolidRect(Vec2(50, 410 - size / 2), Vec2(250, 410 + size / 2), colors[colorOrder]);
			colorOrder = (colorOrder + 1) % 7;
			block->setTag(id);
			//�����ҵ�ź��ڴ��Сid
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
	//ȷ��ָ�û��ִ����
	if (!ds->processList.empty()) {
		int id = ds->processList.front()->id;
		int order = ds->processList.front()->order;
		//����һ��ָ����ʾ����
		auto process = DrawNode::create();
		process->drawCircle(Vec2(480, 550), 100, 360, 100, false, Color4F::BLACK);
		//��ʾָ������������ҵ�źͲ�������
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
	//��ȡ���������ɵĵ�����Ϣ
	Message *m = ds->allocate();
	if (m == NULL) {
		//��ָ��ȫ��ִ����֮��
		//�����ǰҳ��Ϊ�״���Ӧ�㷨
		//����ת�������Ӧ�㷨�Ľ���
		if (ds->getMethod() == DynamicScheduler::FIRST) {
			DynamicScene* scene = DynamicScene::createScene(DynamicScheduler::BEST);
			Director::getInstance()->replaceScene(scene);
		}
		//������ת��������������е�FIFO����
		//FIFO��ָ�������Լ��������
		else {
			auto scene = PageScene::createScene(PageScheduler::FIFOM, NULL);
			Director::getInstance()->replaceScene(scene);
		}
		return;
	}
	addBlock();
	//�Ƴ���һ��ָ��
	DrawNode *n = (DrawNode*)getChildByTag(0);
	n->removeFromParent();
	addProcess();
	//�����ǰָ��������ָ���׼���õ��ڴ���ƶ���ָ����λ��
	if (m->order == DynamicScheduler::APPLY) {
		DrawNode *d = (DrawNode*)getChildByTag(m->id);
		MoveTo *move = MoveTo::create(0.5, Vec2(750, 300 - (m->end - m->start) / 2 - m->start));
		d->runAction(move);
		//����Ӧ�߽�λ��������ʾ����
		//ֻҪ���κ�һ���ڴ��������߽�ͻ�һֱ��ʾ����߽�ֵ
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
		//�ͷ�ָ��ͰѶ�Ӧ�ڴ���Ƴ�
		DrawNode *d = (DrawNode*)getChildByTag(m->id);
		MoveTo *move = MoveTo::create(0.5, Vec2(d->getPosition().x + 500, d->getPosition().y));
		d->runAction(move);
		Label *startNumber = (Label*)getChildByTag(100 + m->start);
		Label *endNumber = (Label*)getChildByTag(100 + m->end);
		//�鿴�Ƿ��б߽���ֵ��Ҫɾȥ
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
