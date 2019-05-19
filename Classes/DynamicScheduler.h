#pragma once
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Block {
	int id;
	int start;
	int length;
	Block(int id, int s, int l) :id(id), start(s), length(l) {};
};

struct Process {
	int id;
	int order;
	int size;
	Process(int id, int o, int s) :id(id), order(o), size(s) {};
};

struct Message {
	int id;
	int start;
	int end;
	int order;
	Message(int id, int s, int e, int o) :id(id), start(s), end(e), order(o) {};
};

class DynamicScheduler
{
private:
	int method;
	vector<Block*> freeMemory;
	vector<Block*> workingMemory;
public:
	static const int FIRST = 0;
	static const int BEST = 1;
	static const int APPLY = 2;
	static const int FREED = 3;
	queue<Process*> processList;
	DynamicScheduler(int method);
	void initProcess();
	Message* allocate();
	void insertWorkingBlock(Block *b);
	void insertFreeBlock(Block *b);
	void mergeFreeBlock();
	int getMethod() { return method; }
	~DynamicScheduler();
};

