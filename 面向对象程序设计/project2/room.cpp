#include "room.h"

using namespace std;

room::room(string name, int e, int w, int u, int d, int id, int hasp, int hasm):east(e),west(w),up(u),down(d) {
    room_name = name;
    room_id = id;
    hasprincess = hasp;
    hasmonster = hasm;
}

room::~room() {};

void room::linkroom(string dir, room *r) {
    nextroom[dir] = r;
}

room room::gonext(string dir) {
    return *nextroom[dir];
}
int room::getroomid() {
    return room_id;
}
int room::cango(string str) {
    if(str=="east"&&east) return 1;
    if(str=="west"&&west) return 1;
    if(str=="up"&&up) return 1;
    if(str=="down"&&down) return 1;
    else return 0;
}
int room::princess() {
    return hasprincess;
}
int room::monster() {
    return hasmonster;
}
void room::checkroom() {                                  //to show the room
    cout << "This is room " << room_name << endl;
    if(up) cout << " __ __" << endl;
    else cout << " _____" << endl;
    if(hasprincess && hasmonster) cout << "| ^ x |" << endl;
    else if(hasprincess) cout << "| ^ ^ |" << endl; 
    else if(hasmonster) cout << "| x x |" << endl;
    else cout << "|     |" << endl;
    if(west) cout << "      ";
    else cout << "|     ";
    if(east) cout << " " << endl;
    else cout << "|" << endl;
    if(down) cout << "|__ __|" << endl;
    else cout << "|_____|" << endl;
}