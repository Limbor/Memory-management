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
	int method;														//����������ȷ���
	float speed = 1.0f;												//�����ٶ�
	bool run = false;												//�Ƿ����ڲ���
	bool finish = false;											//�Ƿ��Ѿ����
	PageScheduler *ps;												//ҳ�������
	Label *hint;													//��ʾ��ǩ
	Label *missingPage;												//ȱҳ����ǩ
	Label *pageFaultRate;											//ȱҳ�ʱ�ǩ
	Label *currentNumber;											//��ǰ��ִ��ָ������
	Label *currentInstruction;										//��ǰָ�����
	Label *insLocation;												//ָ��λ��
	Label *speedTime;												//�����ٶȱ�ǩ
	Label *physicalAddress;											//�����ַ��ǩ
public:
	static PageScene* createScene(int method, int *sequence);		//���캯���Լ�������ʼ��
	virtual bool init();											//Ϊ�����LRU��FIFO���жԱ�
	void enemyInit(int method, int *sequence);						//��ָ������Ҳ��Ϊ����������һ������
	void menuCloseCallback(Ref* pSender);							//�رհ�ť�ص�����
	void addEndButton();											//��ӹرհ�ť
	void nextInstruction(float dt);									//��һ��ָ���ʱ��
	void addPage(Info *inf, int ins);								//�����ҳ��
	CREATE_FUNC(PageScene);
};

#endif // __PAGESCENE_H__