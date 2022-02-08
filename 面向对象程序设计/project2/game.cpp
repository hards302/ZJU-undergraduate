#include "room.h"

int main() {
    int id = 0;                                     //to sign the room, every time creating a room, id increases 1
    int princess, monster;
    bool findpcs = false;
    srand(time(NULL));
    princess = rand()%5+1;                          //pincess will be in room except lobby(0)
    monster = rand()%5+1;                           //so do monster
    /*to create a castle with 6 rooms like
    * ___________
    *|     |     |
    *|  4     5  |
    *|__ __|__ __|
    *|     |     |
    *|  3  |  2  |
    *|__ __|__ __|
    *|     |     |
    *|  0     1  |
    *|__ __|_____|       the number is room_id
    *   ^
    * outside
    */
    room outside("outside", 1, 1, 1, 1, -1, 0, 0);
    room start("lobby", 1, 0, 1, 1, id++, princess==id, monster==id);
    room one("one", 0, 1, 1 ,0, id++, princess==id, monster==id);
    room two("two", 0, 0, 1, 1, id++, princess==id, monster==id);
    room three("three", 0, 0, 1, 1, id++, princess==id, monster==id);
    room four("four", 1, 0, 0, 1, id++, princess==id, monster==id);
    room five("five", 0, 1, 0, 1, id++, princess==id, monster==id);
    //link the lobby room
    start.linkroom("down", &outside);
    start.linkroom("up", &three);
    start.linkroom("east", &one);
    //link room one
    one.linkroom("west", &start);
    one.linkroom("up", &two);
    //link room two
    two.linkroom("up", &five);
    two.linkroom("down", &one);
    //link room three
    three.linkroom("up", &four);
    three.linkroom("down", &start);
    //link room four
    four.linkroom("down", &three);
    four.linkroom("east", &five);
    //link room five
    five.linkroom("down", &two);
    five.linkroom("west", &four);

    //check all the room to see the situation, you can note off them:

    // start.checkroom();
    // one.checkroom();
    // two.checkroom();
    // three.checkroom();
    // four.checkroom();
    // five.checkroom();          

    //game start
    string move, dir;
    cout << "welcome to the castle, you have to save the princess in one of the room.\nBe careful cause the monster may in the castle" << endl;
    cout << "if you're ready, just go up to enter the castle(input 'go up')" << endl;
    cout << "or you can just leave(input 'leave')" << endl;
    getline(cin, move);
    if(move == "leave") {
        cout << "The princess is waiting for you, are you sure?(y/n)";
        char sign;
        cin >> sign;
        if(sign == 'y') return 0;
    }
    room pos = start;
    pos.checkroom();
    cout << "please move(input:go 'dir')" << endl;
    while(pos.getroomid() != -1) {
        cin >> move >> dir;
        if(move == "whereis") {                                            //you can input whereis princess/monster instead go [dir] to check where is princess or monster
            if(dir == "princess") cout << princess << endl;
            else if(dir == "monster") cout << monster << endl;
        }
        else {
            if(pos.cango(dir)) {
                pos = pos.gonext(dir);
                pos.checkroom();
                if(pos.monster()) {
                    cout << "Oh no, the monster is here. You lose!" << endl;
                    return 0;
                }
                else if(pos.princess()) {
                    cout << "You find the princess! Get out of the castle soon!" << endl;
                    findpcs = true;
                }
            }
            else cout << "There is no room, find another way." << endl;
            cout << "please move" << endl;
        }
    }
    if(findpcs) cout << "You win! The princeess and you are safe now." << endl;
    else cout << "Bad luck. You haven't found the princess." << endl;
    return 0;
}