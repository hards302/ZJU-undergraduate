#include<iostream>
#include<map>
#include<string>
#include<vector>
#include<algorithm>
#include<sstream>
#include<iomanip>
using namespace std;
class student {
public:
    string id;
    string name;
    map<string,double> score;
};
class course {
public:
    string name;
    double sumscore;
    int num;
    course(): sumscore(0.0), num(0)
    {}
};
string& trim(string &s) 
{
    if (s.empty()) 
    {
        return s;
    }
 
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}
template <class Type>
Type string2num(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;    
}
map<string, student> stu;
map<string, course> cou;
int main() {
    string line;
    while ( getline(cin, line) ) {
        if ( line.size() <1 ) continue;
        istringstream f(line);
        vector<string> a;
        string w;
        while (getline(f, w, ',') ) {
            trim(w);
            a.push_back(w);
        }
        if(a.size() == 2) {
            stu[a[0]].id = a[0];
            stu[a[0]].name = a[1];
        }
        if(a.size() == 3) {
            stu[a[0]].score[a[1]] = string2num<double>(a[2]);
            cou[a[1]].name = a[1];
            cou[a[1]].num++;
            cou[a[1]].sumscore += string2num<double>(a[2]);
        }
    }
    cout << "student id, name, ";
    for(map<string, course>::iterator p = cou.begin(); p != cou.end(); p++) {
        cout << p->first << ", ";
    }
    cout << "average" << endl;
    double ave = 0.0;
    int am = 0;
    for(map<string, student>::iterator p = stu.begin(); p != stu.end(); p++) {
        cout << p->first << ", " << p->second.name << ", ";
        int num = 0;
        double sum = 0.0;
        for(map<string, course>::iterator q = cou.begin(); q != cou.end(); q++){
            string s = q->first;
            if(p->second.score.find(s) != p->second.score.end()) {
                cout << fixed << setprecision(1) << p->second.score[s];
                num++;
                sum+=p->second.score[s];
            }
            cout << ", ";
        }
        cout << fixed << setprecision(1) << sum/num << endl;
    }
    cout << ", , ";
    for(map<string, course>::iterator p = cou.begin(); p != cou.end(); p++) {
        cout << fixed << setprecision(1) << p->second.sumscore/p->second.num << ", ";
        ave += p->second.sumscore/p->second.num;
        am++;
    }
    cout << fixed << setprecision(1) << ave/am << endl;
    return 0;
}