#ifndef __ROOM_H__
#define __ROOM_H__

#include <iostream>
#include <string>
#include <time.h>
#include <map>

using namespace std;
class room {
private:
    int room_id;                                    //sign the room. different from other room
    string room_name;                                    //room's name
    int east, west, up, down;                       //if it has the door to the direction(0 is not, 1 is)
    map<string, room*> nextroom;                    //the next room. the first string is like "east"/"up"... the room* is the room east/up to this room
    int hasprincess, hasmonster;                    //if this room has princess or monster(0 is not, 1 is)
public:
    // room() {}
    room(string name, int e, int w, int u, int d, int id, int hasp, int hasm);
    ~room();
    void linkroom(string dir, room *r);
    room gonext(string dir);
    int getroomid();
    int cango(string str);
    int princess();
    int monster();
    void checkroom();
};

#endif