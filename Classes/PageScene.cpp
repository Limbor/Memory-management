#include "PageScene.h"
#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"

PageScene* PageScene::createScene(int method, int *sequence)
{
	PageScene* enemy = new PageScene();
	if (enemy && enemy->init())
	{
		enemy->autorelease();
		enemy->enemyInit(method, sequence);
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return NULL;
}

bool PageScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	return true;
}

void PageScene::enemyInit(int method, int *sequence)
{
	this->method = method;
	ps = new PageScheduler(method, sequence);

	auto backgroud = LayerColor::create(Color4B::WHITE);
	this->addChild(backgroud);

	missingPage = Label::createWithSystemFont(String::createWithFormat("MissingPage:%i", ps->getMissingPage())->getCString(), "Arial", 30);
	missingPage->setColor(Color3B::BLACK);
	missingPage->setPosition(150, 750);
	this->addChild(missingPage);

	currentNumber = Label::createWithSystemFont(String::createWithFormat("CurrentNumber:%i", ps->getCurrentNumber())->getCString(), "Arial", 30);
	currentNumber->setColor(Color3B::BLACK);
	currentNumber->setPosition(150, 700);
	this->addChild(currentNumber);

	auto label1 = Label::createWithSystemFont("CurrrenInstruction", "Arial", 30);
	label1->setColor(Color3B::BLACK);
	label1->setPosition(450, 750);
	this->addChild(label1);

	auto label2 = Label::createWithSystemFont((this->method == PageScheduler::FIFOM) ? "FIFO" : "LRU", "Arial", 30);
	label2->setColor(Color3B::BLACK);
	label2->setPosition(800, 700);
	this->addChild(label2);

	currentInstruction = Label::createWithSystemFont(String::createWithFormat("%i", ps->instructionList.front())->getCString(), "Arial", 35);
	currentInstruction->setColor(Color3B::BLACK);
	currentInstruction->setPosition(450, 700);
	this->addChild(currentInstruction);

	pageFaultRate = Label::createWithSystemFont(String::createWithFormat("PageMissingRate:%f%%", 0.0)->getCString(), "Arial", 30);
	pageFaultRate->setColor(Color3B::BLACK);
	pageFaultRate->setPosition(800, 750);
	this->addChild(pageFaultRate);

	addEndButton();

	auto start = ui::Button::create("next.png");
	start->setPosition(Vec2(510, 50));
	start->addClickEventListener([&](Ref* psender) {
		if (finish) {
			if (this->method == PageScheduler::FIFOM) {
				PageScene* scene = PageScene::createScene(PageScheduler::LRUM, ps->sequence);
				Director::getInstance()->replaceScene(scene);
			}
		}
		if (!run) {
			schedule(schedule_selector(PageScene::nextInstruction), 0.1f);
			hint->setString("Pause");
			run = true;
		}
		else {
			unschedule(schedule_selector(PageScene::nextInstruction));
			hint->setString("Start");
			run = false;
		}
	});
	this->addChild(start);

	hint = Label::createWithSystemFont("Start", "Arial", 30);
	hint->setPosition(590, 50);
	hint->setColor(Color3B::BLACK);
	this->addChild(hint);
}

void PageScene::nextInstruction(float dt)
{
	int ins;
	if(!ps->instructionList.empty()) ins = ps->instructionList.front();

	Info *inf;
	if(method == PageScheduler::FIFOM) inf = ps->FIFO();
	else inf = ps->LRU();
	if (inf == NULL) {
		unschedule(schedule_selector(PageScene::nextInstruction));
		hint->setString("Next");
		finish = true;
		return;
	}

	if (insLocation != NULL) insLocation->setColor(Color3B::BLACK);
	currentInstruction->setString(String::createWithFormat("%i", ins)->getCString());
	missingPage->setString(String::createWithFormat("MissingPage:%i", ps->getMissingPage())->getCString());
	currentNumber->setString(String::createWithFormat("CurrentNumber:%i", ps->getCurrentNumber())->getCString());
	double rate = (ps->getCurrentNumber() == 0) ? 0 : ((double)ps->getMissingPage() / (double)ps->getCurrentNumber());
	pageFaultRate->setString(String::createWithFormat("PageMissingRate:%f%%", rate)->getCString());

	if (inf->isFound) {
		DrawNode *d = (DrawNode*)getChildByTag(inf->oldPage);
		insLocation = (Label*)d->getChildByTag(ins - inf->oldPage * 10);
		insLocation->setColor(Color3B::GREEN);
	}
	else {
		if (inf->oldPage == -1) {
			addPage(inf, ins);
		}
		else {
			DrawNode *old = (DrawNode*)getChildByTag(inf->oldPage);
			old->removeFromParent();
			addPage(inf, ins);
		}
	}
}

void PageScene::addPage(Info *inf, int ins)
{
	auto page = DrawNode::create();
	page->drawRect(Vec2(250 * inf->position - 200, 100), Vec2(250 * inf->position, 600), Color4F::BLACK);
	page->setTag(inf->newPage);
	this->addChild(page);
	auto pageNumber = Label::createWithSystemFont(String::createWithFormat("%i", inf->newPage)->getCString(), "Arial", 25);
	pageNumber->setPosition(inf->position * 250 - 100, 625);
	pageNumber->setColor(Color3B::BLACK);
	page->addChild(pageNumber);
	for (int i = 0; i < 10; i++) {
		auto number = Label::createWithSystemFont(String::createWithFormat("%i", inf->newPage * 10 + i)->getCString(), "Arial", 18);
		number->setPosition(inf->position * 250 - 100, 125 + i * 50);
		number->setTag(i);
		if (i == (ins - inf->newPage * 10)) {
			insLocation = number;
			number->setColor(Color3B::GREEN);
		}
		else number->setColor(Color3B::BLACK);
		page->addChild(number);
	}
}

void PageScene::addEndButton()
{
	auto closeItem = MenuItemImage::create("CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(PageScene::menuCloseCallback, this));
	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	float y = origin.y + closeItem->getContentSize().height / 2;
	closeItem->setPosition(Vec2(x, y));
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void PageScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
