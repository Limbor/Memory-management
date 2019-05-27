#include "DynamicScheduler.h"
#include <thread>

DynamicScheduler::DynamicScheduler(int method)
{
	this->method = method;
	initProcess();
	//初始化内存块加入640大小的内存块
	freeMemory.push_back(new Block(0, 0, 640));
}

void DynamicScheduler::initProcess()
{
	int id[11] = { 1,2,3,2,4,3,1,5,6,7,6 };
	char order[11] = { 'a','a','a','f','a','f','f','a','a','a','f' };
	int size[11] = { 130,60,100,60,200,100,130,140,60,50,60 };
	for (int i = 0; i < 11; i++) {
		int o = (order[i] == 'a') ? APPLY : FREED;
		Process *p = new Process(id[i], o, size[i]);
		processList.push(p);
	}
}

Message* DynamicScheduler::allocate()
{
	//如果指令列表为空则返回空值
	if (processList.empty()) return NULL;
	//获取当前指令
	Process *p = processList.front();
	if (p->order == APPLY) {
		//如果为首次适应方法则找出第一个大小符合的内存块
		if (method == FIRST) {
			int start, end;
			Block *newBlock = NULL;
			for (vector<Block*>::iterator i = freeMemory.begin(); i != freeMemory.end(); i++) {
				if ((*i)->length >= p->size) {
					(*i)->id = p->id;
					if ((*i)->length > p->size) {
						int dif = (*i)->length - p->size;
						//更改当前内存块大小
						(*i)->length -= dif;
						//新建空闲内存块
						newBlock = new Block(0, (*i)->start + (*i)->length, dif);
					}
					start = (*i)->start;
					end = start + (*i)->length;
					//插入占用的内存块
					insertWorkingBlock(*i);
					//删除被占用的内存块
					freeMemory.erase(i);
					//若剩余内存块大小不为0则插入新内存块
					if(newBlock) insertFreeBlock(newBlock);
					processList.pop();
					return new Message(p->id, start, end, APPLY);
				}
			}
		}
		else {
			//如果为最佳适应方法则计算每个符合大小的空闲内存块的剩余大小
			//通过差值cost找出最小值
			int cost = INT_MAX;
			vector<Block*>::iterator j;
			for (vector<Block*>::iterator i = freeMemory.begin(); i != freeMemory.end(); i++) {
				if ((*i)->length >= p->size && (*i)->length - p->size < cost) {
					cost = (*i)->length - p->size;
					j = i;
				}
			}
			if (cost != INT_MAX) {
				int start, end;
				Block *newBlock = NULL;
				(*j)->id = p->id;
				if ((*j)->length > p->size) {
					int dif = (*j)->length - p->size;
					(*j)->length -= dif;
					newBlock = new Block(0, (*j)->start + (*j)->length, dif);
				}
				start = (*j)->start;
				end = start + (*j)->length;
				//插入占用的内存块
				insertWorkingBlock(*j);
				//删除当前内存块
				freeMemory.erase(j);
				//插入新的空闲内存快
				if(newBlock) insertFreeBlock(newBlock);
				processList.pop();
				return new Message(p->id, start, end, APPLY);
			}
		}
	}
	else {
		//如果是释放内存操作
		for (vector<Block*>::iterator i = workingMemory.begin(); i != workingMemory.end(); i++) {
			if ((*i)->id == p->id) {
				int start, end;
				(*i)->id = 0;
				start = (*i)->start;
				end = start + (*i)->length;
				//插入空闲内存快
				insertFreeBlock(*i);
				//删除内存块
				workingMemory.erase(i);
				processList.pop();
				return new Message(p->id, start, end, FREED);
			}
		}
	}
	return NULL;
}

void DynamicScheduler::insertWorkingBlock(Block *b) {
	//按起始地址顺序插入
	for (vector<Block*>::iterator i = workingMemory.begin(); i != workingMemory.end(); i++) {
		if ((*i)->start > b->start) {
			workingMemory.insert(i, b);
			return;
		}
	}
	//最大则插入最后一个位置
	workingMemory.insert(workingMemory.end(), b);
}

void DynamicScheduler::insertFreeBlock(Block * b)
{
	for (vector<Block*>::iterator i = freeMemory.begin(); i != freeMemory.end(); i++) {
		if ((*i)->start > b->start) {
			vector<Block*>::iterator j = i;
			freeMemory.insert(i, b);
			//每当插入空闲内存快都要合并周围的空闲内存块
			mergeFreeBlock();
			return;
		}
	}
	freeMemory.insert(freeMemory.end(), b);
}

void DynamicScheduler::mergeFreeBlock()
{
	if (freeMemory.size() < 2) return;
	for (vector<Block*>::iterator i = freeMemory.begin(); (i + 1) != freeMemory.end(); ){
		if ((*(i + 1))->start == (*i)->start + (*i)->length) {
			(*(i + 1))->start = (*i)->start;
			(*(i + 1))->length = (*i)->length + (*(i + 1))->length;
			i = freeMemory.erase(i);
			continue;
		}
		i++;
	}
}


DynamicScheduler::~DynamicScheduler()
{
}
