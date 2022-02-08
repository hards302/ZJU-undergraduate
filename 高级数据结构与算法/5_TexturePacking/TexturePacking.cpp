#include <iostream>
#include <algorithm>
#define MAXN 10010

using namespace std;
//input: the first line:n(the number of rectangle), w(the width of the container)
//       the next n lines: the rectangles' width, the rectangles' height
struct rectangle{                      //the infomation of rectangle
    int width;
    int height;
    int x_axis;
    int y_axis;
};
int cmp1(rectangle a, rectangle b) {   //for sorting rectangles in descending order of height
    return a.height > b.height;
}
int main() {
    int n, w;
    int height = 0;                    //the minimun height of the resulting texture
    int width = 0;                     //the temporary width of the level for putting the rectangle
    int level = 0;                     //the temporary height of the level for putting the rectangle
    int flag = 0;                      //sign of the first rectangle of each level
    cin >> n >> w;
    rectangle rect[MAXN];
    for(int i = 0; i < n; i++) {       //input of rectangles
        cin >> rect[i].width >> rect[i].height;
        #ifdef __EXCHANGE__
        if(rect[i].width < rect[i].height) swap(rect[i].width, rect[i].height);        //if we let the width always be the larger one
        #endif
        if(rect[i].width > w) swap(rect[i].width, rect[i].height);        // need to fit in the strip
    }
    sort(rect, rect+n, cmp1);                       //sort the rectangles
    int index = 0;                                  //index of rectangles
    while(index < n) {
        if(width + rect[index].width <= w) {
            if(flag == 0) {                         //the first rectangle of this level
                rect[index].x_axis = 0;             //this rectangle's x=0,y=height.
                rect[index].y_axis = level;
                height += rect[index].height;       //and increase the height
                width = rect[index].width;          //increase the width
                flag = 1;                           //the first rectangle of this level is settled
            }
            else {
                rect[index].x_axis = width;         //not the first rectangle
                rect[index].y_axis = level;        //just increase the width and x,y's axis
                width += rect[index].width;
            }
            index++;                                //place the next rectangle
        }
        else {
            flag = 0;                               //go to the next level
            width = 0;
            level = height;                         //update the level
        }
    }
    cout << height << endl;                         //ouput the min height
    for(int i = 0; i < n; i++) {
        cout << rect[i].x_axis << " " << rect[i].y_axis << " ";     //output the rectangles' x_axis and y_axis
        cout << rect[i].width << " " << rect[i].height << endl;     //output the rectangles' width and height
    }
    return 0;
}