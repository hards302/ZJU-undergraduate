#pragma once

#include <iostream>
#include <vector>
using namespace std;

typedef struct pair {
	int id;
	double dis;
}ElementType;

class PriorityQueue {
private:
	int Size;
	vector<ElementType> Elements;
	short int getnode[999999999]; // map from id to node in the PQ

	void PercolateUp(int p);
	void PercolateDown(int p);

public:
	PriorityQueue();

	bool IsEmpty(void);
	void Insert(ElementType X);
	ElementType PopMin(void);
	void Update(ElementType X);
};




