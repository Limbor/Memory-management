#include "PageScheduler.h"
#include <random>
#include <ctime>

PageScheduler::PageScheduler(int method, int *sequence)
{
	//FIFO随机生成指令序列
	if(method == FIFOM) init();
	else {
		//LRU拷贝传输的指令序列
		for (int i = 0; i < 320; i++) {
			instructionList.push(sequence[i]);
		}
	}
}

PageScheduler::~PageScheduler()
{
}

void PageScheduler::init()
{
	//记录指令有没有使用过
	int instruction[320] = { 0 };
	static default_random_engine e(unsigned(time(NULL)));
	int random;
	int k = 0, j = 0;
	//指令序列大小为320是结束循环
	while (instructionList.size() < 320) {
		//首先随机取一个数
		if (k == 0) {
			uniform_int_distribution<int> u(0, 319);
			do {
				random = u(e);
			} while (instruction[random]);
			//在该指令左右寻找最近的未使用的指令
			//将这两个指令从小到大插入指令序列中
			//以下两种情况相同
			int right = 1;
			while (instruction[(random + right) % 320]) right++;
			int left = 1;
			while (instruction[(random - left + 320) % 320]) left++;
			if (left < right) {
				right = (random + 320 - (random - left + 320) % 320) % 320;
				random = (random - left + 320) % 320;
			}
			instructionList.push(random);
			instructionList.push((random + right) % 320);
			instruction[random] = 1;
			instruction[(random + right) % 320] = 1;
			sequence[j++] = random;
			sequence[j++] = (random + right) % 320;
		}
		//前地址部分取随机指令
		else if (k == 1 && random > 1) {
			uniform_int_distribution<int> u(0, random - 1);
			int r;
			//当前区间没有未用指令则进入下一个环节
			if (!hasNumber(0, random - 1, instruction)) {
				k = (k + 1) % 3;
				continue;
			}
			do {
				r = u(e);
			} while (instruction[r]);
			int right = 1;
			while (instruction[(r + right) % 320]) right++;
			int left = 1;
			while (instruction[(r - left + 320) % 320]) left++;
			if (left <= right) {
				right = (r + 320 - (r - left + 320) % 320) % 320;
				r = (r - left + 320) % 320;
			}
			instructionList.push(r);
			instructionList.push((r + right) % 320);
			instruction[r] = 1;
			instruction[(r + right) % 320] = 1;
			sequence[j++] = r;
			sequence[j++] = (r + right) % 320;
			random = r;
		}
		//后地址部分随机取指令
		else if (k == 2 && random < 318) {
			uniform_int_distribution<int> u(random + 1, 319);
			if (!hasNumber(random + 1, 319, instruction)) {
				k = (k + 1) % 3;
				continue;
			}
			do {
				random = u(e);
			} while (instruction[random]);
			int right = 1;
			while (instruction[(random + right) % 320]) right++;
			int left = 1;
			while (instruction[(random - left + 320) % 320]) left++;
			if (left < right) {
				right = (random + 320 - (random - left + 320) % 320) % 320;
				random = (random - left + 320) % 320;
			}
			instructionList.push(random);
			instructionList.push((random + right) % 320);
			instruction[random] = 1;
			instruction[(random + right) % 320] = 1;
			sequence[j++] = random;
			sequence[j++] = (random + right) % 320;
		}
		k = (k + 1) % 3;
	}
}


Info* PageScheduler::FIFO() {
	if (instructionList.empty()) return NULL;
	//指令所在页面
	int page = instructionList.front() / 10;
	currentNumber++;
	instructionList.pop();
	//当前页面在内存中
	if (map[page] != 0) {
		return new Info(true, page, map[page]);
	}
	else {
		missingPage++;
		//内存中页面数小于四直接添加
		if (pageQueue.size() < 4) {
			pageQueue.push(page);
			map[page] = pageQueue.size();
			return new Info(false, -1, page, map[page]);
		}
		else {
			//取出将队首元素
			//插入新页面
			int p = pageQueue.front();
			pageQueue.pop();
			pageQueue.push(page);
			map[page] = map[p];
			map[p] = 0;
			return new Info(false, p, page, map[page]);
		}
	}
}

Info* PageScheduler::LRU()
{
	if (instructionList.empty()) return NULL;
	int page = instructionList.front() / 10;
	currentNumber++;
	instructionList.pop();
	if (map[page] != 0) {
		lastTime[map[page] - 1] = currentNumber;
		return new Info(true, page, map[page]);
	}
	else {
		missingPage++;
		if (lastTime.size() < 4) {
			lastTime.push_back(currentNumber);
			map[page] = lastTime.size();
			return new Info(false, -1, page, map[page]);
		}
		else {
			//找出最久未使用的
			int p = searchBest();
			map[page] = map[p];
			map[p] = 0;
			lastTime[map[page] - 1] = currentNumber;
			return new Info(false, p, page, map[page]);
		}
	}
}

bool PageScheduler::hasNumber(int begin, int end, int * instruction)
{
	for (int i = begin; i < end; i++) {
		if (!instruction[i]) return true;
	}
	return false;
}

int PageScheduler::searchBest() {
	int min, cost = 1000;
	for (int i = 0; i < 4; i++) {
		if (lastTime[i] < cost) {
			cost = lastTime[i];
			min = i;
		}
	}
	for (int i = 0; i < 32; i++) {
		if (map[i] == min + 1) return i;
	}
	return -1;
}
