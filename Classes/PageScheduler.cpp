#include "PageScheduler.h"
#include <random>
#include <ctime>

PageScheduler::PageScheduler(int method, int *sequence)
{
	if(method == FIFOM) init();
	else {
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
	int instruction[320] = { 0 };
	static default_random_engine e(unsigned(time(NULL)));
	int random;
	int k = 0, j = 0;
	while (instructionList.size() < 320) {
		if (k == 0) {
			uniform_int_distribution<int> u(0, 319);
			if (hasNextNumber(0, 319, instruction)) {
				do {
					random = u(e);
				} while (instruction[random] || instruction[random + 1]);
				instructionList.push(random);
				instructionList.push(random + 1);
				instruction[random] = 1;
				instruction[random + 1] = 1;
				sequence[j++] = random;
				sequence[j++] = random + 1;
			}
			else {
				do {
					random = u(e);
				} while (instruction[random]);
				instructionList.push(random);
				instruction[random] = 1;
				sequence[j++] = random;
				continue;
			}
		}
		else if (k == 1 && random > 1) {
			uniform_int_distribution<int> u(0, random - 1);
			int r;
			if (!hasNumber(0, random - 1, instruction)) {
				k = (k + 1) % 3;
				continue;
			}
			if (hasNextNumber(0, random - 1, instruction)) {
				do {
					r = u(e);
				} while (instruction[r] || instruction[r + 1]);
				instructionList.push(r);
				instructionList.push(r + 1);
				instruction[r] = 1;
				instruction[r + 1] = 1;
				sequence[j++] = r;
				sequence[j++] = r + 1;
				random = r + 1;
			}
			else {
				do {
					r = u(e);
				} while (instruction[r]);
				instructionList.push(r);
				instruction[r] = 1;
				sequence[j++] = r;
				random = r;
			}
		}
		else if (k == 2 && random < 318) {
			uniform_int_distribution<int> u(random + 1, 319);
			if (!hasNumber(random + 1, 319, instruction)) {
				k = (k + 1) % 3;
				continue;
			}
			if (hasNextNumber(random + 1, 319, instruction)) {
				do {
					random = u(e);
				} while (instruction[random] || instruction[random + 1]);
				instructionList.push(random);
				instructionList.push(random + 1);
				instruction[random] = 1;
				instruction[random + 1] = 1;
				sequence[j++] = random;
				sequence[j++] = random + 1;
			}
			else {
				do {
					random = u(e);
				} while (instruction[random]);
				instructionList.push(random);
				instruction[random] = 1;
				sequence[j++] = random;
			}
		}
		k = (k + 1) % 3;
	}
}

Info* PageScheduler::FIFO() {
	if (instructionList.empty()) return NULL;
	int page = instructionList.front() / 10;
	currentNumber++;
	instructionList.pop();
	if (map[page] != 0) {
		return new Info(true, page);
	}
	else {
		missingPage++;
		if (pageQueue.size() < 4) {
			pageQueue.push(page);
			map[page] = pageQueue.size();
			return new Info(false, -1, page, map[page]);
		}
		else {
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
		return new Info(true, page);
	}
	else {
		missingPage++;
		if (lastTime.size() < 4) {
			lastTime.push_back(currentNumber);
			map[page] = lastTime.size();
			return new Info(false, -1, page, map[page]);
		}
		else {
			int p = searchBest();
			map[page] = map[p];
			map[p] = 0;
			lastTime[map[page] - 1] = currentNumber;
			return new Info(false, p, page, map[page]);
		}
	}
}

bool PageScheduler::hasNextNumber(int begin, int end, int *instruction)
{
	for (int i = begin; i < end; i++) {
		if (!instruction[i] && !instruction[i + 1]) return true;
	}
	return false;
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
