#pragma once
#include <queue>
#include <vector>

using namespace std;

struct Info {
	bool isFound;
	int oldPage;
	int newPage;
	int position;
	Info(bool b, int o) :isFound(b), oldPage(o) {};
	Info(bool b, int o, int n, int p) :isFound(b), oldPage(o), newPage(n), position(p) {};
};

class PageScheduler
{
private:
	int map[32] = { 0 };
	int currentNumber = 0;
	int missingPage = 0;
	queue<int> pageQueue;
	vector<int> lastTime;
public:
	static const int LRUM = 1;
	static const int FIFOM = 2;
	int sequence[320] = { 0 };
	queue<int> instructionList;

	PageScheduler(int method, int *sequence);
	~PageScheduler();
	void init();
	Info* FIFO();
	Info* LRU();
	bool hasNextNumber(int begin, int end, int *instruction);
	bool hasNumber(int begin, int end, int *instruction);
	int searchBest();
	int getMissingPage() { return missingPage; }
	int getCurrentNumber() { return currentNumber; }
};