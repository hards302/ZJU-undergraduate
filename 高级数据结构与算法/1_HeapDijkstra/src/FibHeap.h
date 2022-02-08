#pragma once
#include<stdio.h>
#include<stdlib.h>

typedef int KeyType;    // to store distance info
typedef int NameType;   // to store vertex name info
// Info means meaningful info to carry within a node
typedef struct Info 
{
    NameType name;
    KeyType key;
}Info;

typedef struct FNode FNode;
struct FNode
{
    Info info;
    FNode *child;
    FNode *fsib,*nsib;  // need circular-double linked-list
    FNode *parent;
    int deg;            // counting # of childs
    int loser;          // mark if this node has lost a child
};

typedef FNode* Root;
typedef struct FibHeap FibHeap;
struct FibHeap
{
    Root head;
    Root min;
};

void Insert(FibHeap *H, Info info, Root *map_heap);
Info Popmin(FibHeap *H,Root *map_heap);
void Decrease_key(FibHeap *H, Info ninfo, Root *map_heap);

