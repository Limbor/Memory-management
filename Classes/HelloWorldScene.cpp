#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
	auto backgroud = LayerColor::create(Color4B::WHITE);
	this->addChild(backgroud);

	addEndButton();

	auto memory = DrawNode::create();
	memory->drawRect(Vec2(800, 70), Vec2(1000, 710), Color4F::BLACK);
	this->addChild(memory);

	addBlock();

	auto next = ui::Button::create("CloseNormal.png", "CloseSelected.png");
	next->setPosition(Vec2(500 - next->getContentSize().width / 2, 410 - next->getContentSize().height / 2));
	next->addClickEventListener(CC_CALLBACK_1(HelloWorld::nextCallback, this));
	this->addChild(next);
    return true;
}

void HelloWorld::addBlock() {
	if (!ds->processList.empty()) {
		if (ds->processList.front()->order == DynamicScheduler::APPLY) {
			int id = ds->processList.front()->id;
			int size = ds->processList.front()->size;
			auto block = DrawNode::create();
			block->drawSolidRect(Vec2(50, 410 - size / 2), Vec2(250, 410 + size / 2), colors[colorOrder]);
			colorOrder = (colorOrder + 1) % 7;
			block->setTag(id);
			auto idNumber = Label::createWithSystemFont(String::createWithFormat("%i", id)->getCString(), "Arial", 30);
			idNumber->setPosition(150,410);
			idNumber->setColor(Color3B::WHITE);
			block->addChild(idNumber);
			this->addChild(block);
		}
	}
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::nextCallback(Ref * pSender)
{
	Message *m = ds->allocate();
	if (m == NULL) {
		ui::Button *b = (ui::Button*)pSender;
		b->setEnabled(true);
		return;
	}
	addBlock();
	if (m->order == DynamicScheduler::APPLY) {
		DrawNode *d = (DrawNode*)getChildByTag(m->id);
		MoveTo *move = MoveTo::create(1.0, Vec2(750, 300 - (m->end - m->start) / 2 - m->start));
		d->runAction(move);
		if (number[m->start] == 0) {
			auto startNumber = Label::createWithSystemFont(String::createWithFormat("%i", m->start)->getCString(), "Arial", 18);
			startNumber->setPosition(750, 710 - m->start);
			startNumber->setColor(Color3B::BLACK);
			startNumber->setTag(100 + m->start);
			this->addChild(startNumber);
		}
		number[m->start]++;
		if (number[m->end] == 0) {
			auto endNumber = Label::createWithSystemFont(String::createWithFormat("%i", m->end)->getCString(), "Arial", 18);
			endNumber->setPosition(750, 710 - m->end);
			endNumber->setColor(Color3B::BLACK);
			endNumber->setTag(100 + m->end);
			this->addChild(endNumber);
		}
		number[m->end]++;
	}
	else {
		DrawNode *d = (DrawNode*)getChildByTag(m->id);
		MoveTo *move = MoveTo::create(1, Vec2(d->getPosition().x + 500, d->getPosition().y));
		d->runAction(move);
		Label *startNumber = (Label*)getChildByTag(100 + m->start);
		Label *endNumber = (Label*)getChildByTag(100 + m->end);
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

void HelloWorld::addEndButton()
{
	auto closeItem = MenuItemImage::create("CloseNormal.png",
										   "CloseSelected.png",
										   CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}
