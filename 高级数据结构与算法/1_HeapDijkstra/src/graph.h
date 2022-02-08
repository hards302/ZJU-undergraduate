#ifndef _GRAPH_H_
#define _GRAPH_H_

#include<iostream>
#include<vector>
using namespace std;
struct node {
    int val;
    double weight;
    node *next;
};
class point {
public:
    double dis;
    bool visit;
    point() {
        dis = 0;
        visit = false;
    }
};
class Graph {
private:
    int v_num;
    int e_num;
    vector<node*> edge;
    vector<point> dot;
public:
    Graph(int v,int e);
    void createG();
    void dijkstra(int start);
    void print();
};
#endif 