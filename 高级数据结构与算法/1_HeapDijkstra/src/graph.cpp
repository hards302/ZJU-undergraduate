#include"graph.h"
#if Bin
#include"./BinomialQueue.h"
#include"./BinomialQueue.cpp"
#else
#include"./FibHeap.h"
#include"./FibHeap.c"
#endif
int inf = 999999999;
Graph::Graph(int v, int e) {
    this->v_num = v;
    this->e_num = e;
    edge.resize(v+1);
    for(int i = 0; i <= v; i++) edge[i] = NULL;
    dot.resize(v+1);
}
void Graph::createG() {
    cout << "inCreateG" << endl;
    int start, end, weight;
    int isdirect;
    cout << "if this graph is direct, please input 1, otherwise input 0:";
    cin >> isdirect;
    cout << "input edges as:start end weight:" << endl;
    for(int i = 0; i < this->e_num; i++) {
        cin >> start >> end >> weight;
        node *temp = edge[start];
        if(temp == NULL) {
            edge[start] = new node();
            edge[start]->val = end;
            edge[start]->weight = weight;
            edge[start]->next = NULL;
        }
        else {
            while(temp->next != NULL) temp = temp->next;
            node *now = new node();
            now->val = end;
            now->weight = weight;
            now->next = NULL;
            temp->next = now;
        }
        if(!isdirect) {
            temp = edge[end];
            if(temp == NULL) {
                edge[end] = new node();
                edge[end]->val = start;
                edge[end]->weight = weight;
                edge[end]->next = NULL;
            }
            else {
                while(temp->next != NULL) temp = temp->next;
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
    cout << "indijkstra" << endl;
    Root *map_heap;
    map_heap = new Root[this->v_num+1];
    vector<int> check(this->v_num+1);
    FibHeap *pq;
    Info temp;
    node *tempnode = edge[start];
    for(int i = 0; i <= this->v_num; i++) {
        dot[i].dis = inf;
    }
    while(tempnode != NULL) {
        dot[tempnode->val].dis = tempnode->weight;
        tempnode = tempnode->next;
    }
    for(int i = 0; i < this->v_num; i++) check[i] = 0;
    dot[start].dis = 0;
    dot[start].visit = true;
    temp.name = start; temp.key = 0;
    Insert(pq, temp, map_heap);
    check[start] = 1;
    if(!pq) {
        int point = Popmin(pq, map_heap).name;
        dot[point].dis = Popmin(pq, map_heap).key;
        dot[point].visit = true;
        for(tempnode = edge[point]; tempnode != NULL; tempnode = tempnode->next) {
            if(!dot[tempnode->val].visit) {
                temp.name = tempnode->val;
                temp.key = tempnode->weight;
                if(!check[tempnode->val]) {
                    check[tempnode->val] = 1;
                    Insert(pq, temp, map_heap);
                }
                else Decrease_key(pq, temp, map_heap);
            }
        }
    }
}
void Graph::print() {
    for(int i = 1; i <= this->v_num; i++) {
        if(dot[i].dis != inf) {
            cout << i << ":" << dot[i].dis << endl;
        }
        else cout << i << ":" << "no path to this place" << endl;
    }
}