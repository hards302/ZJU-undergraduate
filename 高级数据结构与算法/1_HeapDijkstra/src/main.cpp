#include "./graph.h"
#include "./graph.cpp"
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