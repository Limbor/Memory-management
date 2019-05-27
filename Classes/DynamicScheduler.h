#pragma once
#include <iostream>
#include <vector>
#include <queue>

using namespace std;
//内存块结构体
struct Block {
	int id;			//当前占用的作业号若空闲则为0
	int start;		//起始地址
	int length;		//大小
	Block(int id, int s, int l) :id(id), start(s), length(l) {};
};
//指令结构体
struct Process {
	int id;			//操作作业号
	int order;		//操作类型
	int size;		//作业所需大小
	Process(int id, int o, int s) :id(id), order(o), size(s) {};
};
//调度信息结构体
struct Message {
	int id;			//作业号
	int start;		//起始地址
	int end;		//终止地址
	int order;		//操作类型
	Message(int id, int s, int e, int o) :id(id), start(s), end(e), order(o) {};
};

class DynamicScheduler
{
private:
	int method;								//调度方式
	vector<Block*> freeMemory;				//空闲内存块
	vector<Block*> workingMemory;			//占用的内存块
public:
	static const int FIRST = 0;				//首次适应
	static const int BEST = 1;				//最佳适应
	static const int APPLY = 2;				//申请内存指令
	static const int FREED = 3;				//释放内存指令
	queue<Process*> processList;			//指令队列
	DynamicScheduler(int method);			//构造函数
	void initProcess();						//初始化指令队列
	Message* allocate();					//调度算法
	void insertWorkingBlock(Block *b);		//插入被占用的内存块
	void insertFreeBlock(Block *b);			//插入空闲的内存块
	void mergeFreeBlock();					//合并空闲内存块
	int getMethod() { return method; }
	~DynamicScheduler();
};

