#include "DynamicScheduler.h"
#include <thread>

DynamicScheduler::DynamicScheduler(int method)
{
	this->method = method;
	initProcess();
	//��ʼ���ڴ�����640��С���ڴ��
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
	//���ָ���б�Ϊ���򷵻ؿ�ֵ
	if (processList.empty()) return NULL;
	//��ȡ��ǰָ��
	Process *p = processList.front();
	if (p->order == APPLY) {
		//���Ϊ�״���Ӧ�������ҳ���һ����С���ϵ��ڴ��
		if (method == FIRST) {
			int start, end;
			Block *newBlock = NULL;
			for (vector<Block*>::iterator i = freeMemory.begin(); i != freeMemory.end(); i++) {
				if ((*i)->length >= p->size) {
					(*i)->id = p->id;
					if ((*i)->length > p->size) {
						int dif = (*i)->length - p->size;
						//���ĵ�ǰ�ڴ���С
						(*i)->length -= dif;
						//�½������ڴ��
						newBlock = new Block(0, (*i)->start + (*i)->length, dif);
					}
					start = (*i)->start;
					end = start + (*i)->length;
					//����ռ�õ��ڴ��
					insertWorkingBlock(*i);
					//ɾ����ռ�õ��ڴ��
					freeMemory.erase(i);
					//��ʣ���ڴ���С��Ϊ0��������ڴ��
					if(newBlock) insertFreeBlock(newBlock);
					processList.pop();
					return new Message(p->id, start, end, APPLY);
				}
			}
		}
		else {
			//���Ϊ�����Ӧ���������ÿ�����ϴ�С�Ŀ����ڴ���ʣ���С
			//ͨ����ֵcost�ҳ���Сֵ
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
				//����ռ�õ��ڴ��
				insertWorkingBlock(*j);
				//ɾ����ǰ�ڴ��
				freeMemory.erase(j);
				//�����µĿ����ڴ��
				if(newBlock) insertFreeBlock(newBlock);
				processList.pop();
				return new Message(p->id, start, end, APPLY);
			}
		}
	}
	else {
		//������ͷ��ڴ����
		for (vector<Block*>::iterator i = workingMemory.begin(); i != workingMemory.end(); i++) {
			if ((*i)->id == p->id) {
				int start, end;
				(*i)->id = 0;
				start = (*i)->start;
				end = start + (*i)->length;
				//��������ڴ��
				insertFreeBlock(*i);
				//ɾ���ڴ��
				workingMemory.erase(i);
				processList.pop();
				return new Message(p->id, start, end, FREED);
			}
		}
	}
	return NULL;
}

void DynamicScheduler::insertWorkingBlock(Block *b) {
	//����ʼ��ַ˳�����
	for (vector<Block*>::iterator i = workingMemory.begin(); i != workingMemory.end(); i++) {
		if ((*i)->start > b->start) {
			workingMemory.insert(i, b);
			return;
		}
	}
	//�����������һ��λ��
	workingMemory.insert(workingMemory.end(), b);
}

void DynamicScheduler::insertFreeBlock(Block * b)
{
	for (vector<Block*>::iterator i = freeMemory.begin(); i != freeMemory.end(); i++) {
		if ((*i)->start > b->start) {
			vector<Block*>::iterator j = i;
			freeMemory.insert(i, b);
			//ÿ����������ڴ�춼Ҫ�ϲ���Χ�Ŀ����ڴ��
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
