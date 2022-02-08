//#include "graph.h"
#include"BinaryHeap.h"
#include"BinaryHeap.cpp"
//////////////////////////////////////////
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
	Graph(int v, int e);
	void createG();
	void dijkstra(int start);
	void print();
};
int inf = 999999999;

Graph::Graph(int v, int e) {
	this->v_num = v;
	this->e_num = e;
	edge.resize(v + 1);
	for (int i = 0; i <= v; i++) edge[i] = NULL;
	dot.resize(v + 1);
}
void Graph::createG() {
	int start, end, weight;
	int isdirect;
	cout << "if this graph is direct, please input 1, otherwise input 0:";
	cin >> isdirect;
	cout << "input edges as:start end weight:" << endl;
	for (int i = 0; i < this->e_num; i++) {
		cin >> start >> end >> weight;
		node *temp = edge[start];
		if (temp == NULL) {
			edge[start] = new node();
			edge[start]->val = end;
			edge[start]->weight = weight;
			edge[start]->next = NULL;
		}
		else {
			while (temp->next != NULL) temp = temp->next;
			node *now = new node();
			now->val = end;
			now->weight = weight;
			now->next = NULL;
			temp->next = now;
		}
		if (!isdirect) {
			temp = edge[end];
			if (temp == NULL) {
				edge[end] = new node();
				edge[end]->val = start;
				edge[end]->weight = weight;
				edge[end]->next = NULL;
			}
			else {
				while (temp->next != NULL) temp = temp->next;
				node *now = new node();
				now->val = start;
				now->weight = weight;
				now->next = NULL;
				temp->next = now;
			}
		}
	}
}
void Graph::dijkstra(int start) {
	vector<int> check(this->v_num + 1);
	PriorityQueue pq;
	ElementType temp;
	node *tempnode = edge[start];
	for (int i = 0; i <= this->v_num; i++) {
		dot[i].dis = inf;
	}
	while (tempnode != NULL) {
		dot[tempnode->val].dis = tempnode->weight;
		tempnode = tempnode->next;
	}
	for (int i = 0; i < this->v_num; i++) check[i] = 0;
	dot[start].dis = 0;
	dot[start].visit = true;
	temp.id = start; temp.dis = 0;
	pq.Insert(temp);
	check[start] = 1;
	if (!pq.IsEmpty()) {
		int point = pq.PopMin().id;
		dot[point].dis = pq.PopMin().dis;
		dot[point].visit = true;
		for (tempnode = edge[point]; tempnode != NULL; tempnode = tempnode->next) {
			if (!dot[tempnode->val].visit) {
				temp.id = tempnode->val;
				temp.dis = tempnode->weight;
				if (!check[tempnode->val]) {
					check[tempnode->val] = 1;
					pq.Insert(temp);
				}
				else pq.Update(temp);
			}
		}
	}
}
void Graph::print() {
	for (int i = 1; i <= this->v_num; i++) {
		if (dot[i].dis != inf) {
			cout << i << ":" << dot[i].dis << endl;
		}
		else cout << i << ":" << "no path to this place" << endl;
	}
}
//////////////////////////////////////////

int main() {
    int v, e;
    cout << "input vertex number and edge number:" << endl;
    cin >> v >> e;
    Graph graph(v, e);
    graph.createG();
    cout << "input the start place:" << endl;
    int start;
    cin >> start;
    graph.dijkstra(start);
    graph.print();
    return 0;
}