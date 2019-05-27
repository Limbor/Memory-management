#pragma once
#include <iostream>
#include <vector>
#include <queue>

using namespace std;
//�ڴ��ṹ��
struct Block {
	int id;			//��ǰռ�õ���ҵ����������Ϊ0
	int start;		//��ʼ��ַ
	int length;		//��С
	Block(int id, int s, int l) :id(id), start(s), length(l) {};
};
//ָ��ṹ��
struct Process {
	int id;			//������ҵ��
	int order;		//��������
	int size;		//��ҵ�����С
	Process(int id, int o, int s) :id(id), order(o), size(s) {};
};
//������Ϣ�ṹ��
struct Message {
	int id;			//��ҵ��
	int start;		//��ʼ��ַ
	int end;		//��ֹ��ַ
	int order;		//��������
	Message(int id, int s, int e, int o) :id(id), start(s), end(e), order(o) {};
};

class DynamicScheduler
{
private:
	int method;								//���ȷ�ʽ
	vector<Block*> freeMemory;				//�����ڴ��
	vector<Block*> workingMemory;			//ռ�õ��ڴ��
public:
	static const int FIRST = 0;				//�״���Ӧ
	static const int BEST = 1;				//�����Ӧ
	static const int APPLY = 2;				//�����ڴ�ָ��
	static const int FREED = 3;				//�ͷ��ڴ�ָ��
	queue<Process*> processList;			//ָ�����
	DynamicScheduler(int method);			//���캯��
	void initProcess();						//��ʼ��ָ�����
	Message* allocate();					//�����㷨
	void insertWorkingBlock(Block *b);		//���뱻ռ�õ��ڴ��
	void insertFreeBlock(Block *b);			//������е��ڴ��
	void mergeFreeBlock();					//�ϲ������ڴ��
	int getMethod() { return method; }
	~DynamicScheduler();
};

