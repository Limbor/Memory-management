#pragma once
#include <queue>
#include <vector>

using namespace std;
//������Ϣ�ṹ��
struct Info {
	bool isFound;						//�Ƿ��ҵ�
	int oldPage;						//���滻����ҳ����������Ϊ-1
	int newPage;						//�¼�ҳ����
	int position;						//ҳ��λ���ڴ��е�λ��
	Info(bool b, int o, int p) :isFound(b), oldPage(o), position(p) {};
	Info(bool b, int o, int n, int p) :isFound(b), oldPage(o), newPage(n), position(p) {};
};

class PageScheduler
{
private:
	int map[32] = { 0 };									//��¼�ڴ��е�ҳ�漰λ��
	int currentNumber = 0;									//��ǰִ��ָ������
	int missingPage = 0;									//ȱҳ��
	queue<int> pageQueue;									//FIFO��ҳ�����
	vector<int> lastTime;									//LRU�м�¼ҳ���ϴ�ʹ��ʱ��
public:
	static const int LRUM = 1;								//LRU����
	static const int FIFOM = 2;								//FIFO����
	int sequence[320] = { 0 };								//ָ���������ڴ��ݸ���һ������
	queue<int> instructionList;								//ָ�����

	PageScheduler(int method, int *sequence);
	~PageScheduler();
	void init();											//ָ�����г�ʼ��
	Info* FIFO();											//FIFO���ȷ���
	Info* LRU();											//LRU���ȷ���
	bool hasNumber(int begin, int end, int *instruction);	//��ǰ���仹���޻�δ�������е�ָ��
	int searchBest();										//LRU��Ѱ�ұ��滻��ҳ��
	int getMissingPage() { return missingPage; }
	int getCurrentNumber() { return currentNumber; }
};