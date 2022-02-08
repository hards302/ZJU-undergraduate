#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef int KeyType ;
typedef struct Node Node;
struct Node
{
    KeyType key;
    Node* lchild;
    Node* nsib;
    Node* parent;
    int deg;    // degree of this node; mark how many child the node have
    int loser;  // mark if a node losing a child
};
// Node constructor
void Node_construct(Node* pnode, KeyType value);

typedef Node* Root;
typedef struct LNode LNode;
// list 应该实现为双向链表
struct LNode
{
    Root p;
    LNode* next;
    LNode* forward;
};
// LNode constructor
void LNode_construct(LNode* plnode, Root root);

typedef struct FibHeap FibHeap;
struct FibHeap
{
    LNode* head;
    LNode* minroot;
};
// FibHeap constructor
void FibHeap_construct(FibHeap* H);

void Insert(FibHeap* H, double value);
// popmin 应该要返回地名
void Popmin(FibHeap* H);
void Update_key(FibHeap* H, Node* pnode,KeyType nvalue);