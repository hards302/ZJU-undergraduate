#include "BinomialQueue.h"

BinQueue Insert(BinQueue H, ElementType X)
{
	BinQueue H2 = InitialBinQueue();
	H2->CurrentSize++;
	H2->TheTrees[0]->Element = X;

	return H = Merge(H, H2);
}

ElementType PopMin(BinQueue H)
{
	BinQueue DeletedQueue;
	Position DeletedTree, OldRoot;
	ElementType MinItem = Infinity;						/* the minimum item to be returned */
	int i, j, MinTree;									/* MinTree is the index of the tree with the minimum item */

	if (IsEmpty(H)) { printf("The Binomial Queue is empty.\n");  return -Infinity; }

	for (i = 0; i < MaxTrees; i++) {					/* Step 1: find the minimum item */
		if (H->TheTrees[i] && H->TheTrees[i]->Element < MinItem) {
			MinItem = H->TheTrees[i]->Element;  MinTree = i;
		}
	}
	DeletedTree = H->TheTrees[MinTree];					/* Step 2: remove the MinTree from H => H’ */
	H->TheTrees[MinTree] = NULL;					 
	
	OldRoot = DeletedTree;								/* Step 3.1: remove the root */
	DeletedTree = DeletedTree->LeftChild;   free(OldRoot);

	DeletedQueue = InitialBinQueue();					/* Step 3.2: create H” */
	DeletedQueue->CurrentSize = (1 << MinTree) - 1;
	for (j = MinTree - 1; j >= 0; j--) {
		DeletedQueue->TheTrees[j] = DeletedTree;
		DeletedTree = DeletedTree->NextSibling;
		DeletedQueue->TheTrees[j]->NextSibling = NULL;
		DeletedQueue->TheTrees[j]->Parent = NULL;//
	}
	H->CurrentSize -= DeletedQueue->CurrentSize + 1;

	H = Merge(H, DeletedQueue);							/* Step 4: merge H’ and H” */
	return MinItem;
}

void Update(BinQueue H, Position p, ElementType X)
{
	if (p == NULL) { printf("Nothing to update.\n"); return; }
	if (p->Element < X) {
		while (p->Parent && p->Element < p->Parent->Element) {
			SwapVal(p, p->Parent);
		}
	}
	if (p->Element > X) {
		while (p->LeftChild && p->Element < p->LeftChild->Element) {
			SwapVal(p, p->LeftChild);
		}
	}
	return;
}

int FindELemTree(BinQueue H, ElementType X)			//返回目标元素所在树的索引 (从0起) 假设元素不重复
{
	Position p, q;
	for (int i = 0; i < MaxTrees; i++){
		p = H->TheTrees[i];
		while (p) {
			if (p->Element == X)return i;
			
			q = p->NextSibling;
			while (q) {
				if (q->Element == X) return i;
				q = q->NextSibling;
			}
			p = p->LeftChild;
		}
	}
	return -1;		//return -1 if not found
}

Position FindELemPos(BinQueue H, ElementType X)	//返回目标元素的Position
{
	Position p, q;
	int i = FindELemTree(H, X);
	p = H->TheTrees[i];
	while (p) {
		if (p->Element == X)return p;

		q = p->NextSibling;
		while (q) {
			if (q->Element == X) return q;
			q = q->NextSibling;
		}
		p = p->LeftChild;
	}
	return NULL;		//return NULL if not found
}

BinTree InitialBinQueue(void)
{
	BinQueue H = (struct Collection*)malloc(sizeof(struct Collection));
	H->CurrentSize = 0;
	for (int i = 0; i < MaxTrees; i++) {
		H->TheTrees[i]->Element = -1;
		H->TheTrees[i]->LeftChild = NULL;
		H->TheTrees[i]->NextSibling = NULL;
		H->TheTrees[i]->Parent = NULL;
	}
	return H;
}

BinTree CombineTrees(BinTree T1, BinTree T2)
{  /* merge equal-sized T1 and T2 */
	if (T1->Element > T2->Element)
		return CombineTrees(T2, T1);

	/* insert T2 to the front of the children list of T1 */
	T2->NextSibling = T1->LeftChild;
	T1->LeftChild = T2;
	T2->Parent = T1;//
	return T1;
}

BinQueue Merge(BinQueue H1, BinQueue H2)	
{
	BinTree T1, T2, Carry = NULL;
	int i, j;
	if (H1->CurrentSize + H2->CurrentSize > Capacity)  printf("out of capacity.\n");

	H1->CurrentSize += H2->CurrentSize;
	for (i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2) {
		T1 = H1->TheTrees[i]; T2 = H2->TheTrees[i]; 
		switch (4 * !!Carry + 2 * !!T2 + !!T1) {
		case 0: /* 000 */
		case 1: /* 001 */  break;
		case 2: /* 010 */  H1->TheTrees[i] = T2; H2->TheTrees[i] = NULL; break;
		case 4: /* 100 */  H1->TheTrees[i] = Carry; Carry = NULL; break;
		case 3: /* 011 */  Carry = CombineTrees(T1, T2);
			H1->TheTrees[i] = H2->TheTrees[i] = NULL; break;
		case 5: /* 101 */  Carry = CombineTrees(T1, Carry);
			H1->TheTrees[i] = NULL; break;
		case 6: /* 110 */  Carry = CombineTrees(T2, Carry);
			H2->TheTrees[i] = NULL; break;
		case 7: /* 111 */  H1->TheTrees[i] = Carry;
			Carry = CombineTrees(T1, T2);
			H2->TheTrees[i] = NULL; break;
		} /* end switch */
	} /* end for-loop */
	return H1;
}

void SwapVal(Position p, Position q) 
{
	ElementType t;
	t = p->Element;
	p->Element = q;
	q->Element = t;
}