#include "BinaryHeap.h"

using namespace std;

void PriorityQueue::Insert(ElementType  X)
{
	int i;
	this->Size++;
	this->Elements.push_back(X);
	getnode[X.id] = Elements.size();

	PercolateUp(this->Size);

	return;
}
ElementType PriorityQueue::PopMin(void)
{
	if (IsEmpty()) { cout << "Priority queue is empty.\n"; exit(0); }

	int i, SmallChild;
	getnode[this->Elements[this->Size].id] = 0;
	ElementType X = this->Elements[this->Size--];
	this->Elements.erase(this->Elements.end());
	
	getnode[this->Elements[1].id] = 0;
	this->Elements[1] = X;
	getnode[X.id] = 1;

	PercolateDown(1);

	return X;
}
void PriorityQueue::Update(ElementType X)
{
	int p = getnode[X.id];

	if (X.dis > this->Elements[p].dis)
		return;

	if (X.dis < this->Elements[p].dis) {
		getnode[this->Elements[p].id] = 0;
		this->Elements[p] = X;
		getnode[X.id] = p;
		PercolateUp(p);
	}
	
	return;
}
void PriorityQueue::PercolateUp(int p)
{
	ElementType X = this->Elements[p];
	while (X.dis < this->Elements[p / 2].dis) {
		getnode[this->Elements[p / 2].id] = p;
		this->Elements[p] = this->Elements[p / 2];
		getnode[this->Elements[p].id] = 0;

		p = p / 2;
	}
	this->Elements[p] = X;
	return;
}
void PriorityQueue::PercolateDown(int p)
{
	int i, SmallChild;
	ElementType X = this->Elements[p];

	for (i = p; i * 2 <= this->Size; i = SmallChild) { /* 如果不是叶(=有子=有左子)*/
		SmallChild = i * 2;
		if (SmallChild != this->Size  &&  this->Elements[SmallChild + 1].dis < this->Elements[SmallChild].dis)
			SmallChild++;
		/* 至此SmallChild已为较小的子节点 */

		if (X.dis > this->Elements[SmallChild].dis) {
			getnode[this->Elements[SmallChild].id] = i;
			this->Elements[i] = this->Elements[SmallChild];
			getnode[this->Elements[i].id] = 0;
		}
		else break;
	}
	this->Elements[i] = X;

	return;
}
bool PriorityQueue::IsEmpty(void)
{
	if (this->Size == 0) return true;
	else return false;
}
PriorityQueue::PriorityQueue()
{
	Size = 0;
	return;
}
