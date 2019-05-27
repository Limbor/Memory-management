#pragma once
#include <queue>
#include <vector>

using namespace std;
//传输信息结构体
struct Info {
	bool isFound;						//是否找到
	int oldPage;						//被替换掉的页面编号若无则为-1
	int newPage;						//新加页面编号
	int position;						//页面位于内存中的位置
	Info(bool b, int o, int p) :isFound(b), oldPage(o), position(p) {};
	Info(bool b, int o, int n, int p) :isFound(b), oldPage(o), newPage(n), position(p) {};
};

class PageScheduler
{
private:
	int map[32] = { 0 };									//记录内存中的页面及位置
	int currentNumber = 0;									//当前执行指令条数
	int missingPage = 0;									//缺页数
	queue<int> pageQueue;									//FIFO中页面队列
	vector<int> lastTime;									//LRU中记录页面上次使用时间
public:
	static const int LRUM = 1;								//LRU方法
	static const int FIFOM = 2;								//FIFO方法
	int sequence[320] = { 0 };								//指令序列用于传递给下一个场景
	queue<int> instructionList;								//指令队列

	PageScheduler(int method, int *sequence);
	~PageScheduler();
	void init();											//指令序列初始化
	Info* FIFO();											//FIFO调度方法
	Info* LRU();											//LRU调度方法
	bool hasNumber(int begin, int end, int *instruction);	//当前区间还有无还未纳入序列的指令
	int searchBest();										//LRU中寻找被替换的页面
	int getMissingPage() { return missingPage; }
	int getCurrentNumber() { return currentNumber; }
};