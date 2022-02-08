#include<iostream>
#include<vector>
#include<algorithm>
#include<time.h>
#include<chrono>

using namespace std;

typedef int KeyType;
typedef int ValueType;


// SkipList could be the implementation of a dict.
// therefore we store key-val pair inside
struct Node
{
    KeyType key;
    ValueType val;
    vector<Node*> next;
};

class SkipList
{
    Node* H;
    const int SIZE = 32;
    public: 
        SkipList();     // init empty sklist
        ~SkipList();    
        friend ostream& operator<<(ostream& s, SkipList& L);
        ValueType Search(KeyType key);
        void Update(KeyType key,ValueType val);
        bool Delete(KeyType key);
};

SkipList::SkipList()
{
    H = new Node;
    H->next.push_back(nullptr);
}
SkipList::~SkipList()
{
    // like a singly-linked list free
    // use the 0th level to traverse and free each node
    Node *tmp=H,*tmpn = H->next[0];
    while (tmpn)
    {
        delete tmp;
        tmp = tmpn;
        tmpn = tmpn->next[0];
    }
    delete tmp;
}

ostream& operator<<(ostream& s, SkipList& L)
{
    // like printing lists, from lowest level
    // print (key:val) pair to demo the update
    Node* H = L.H;
    for(int i = 0;i<H->next.size(); ++i)
    {
        Node* tmp = H->next[i];
        while(tmp)
        {
            s<< "("<<tmp->key<<" : "<<tmp->val<<")"<<" ";
            tmp = tmp->next[i];
        }
        s<<"\n";
    }
    return s;
}

ValueType SkipList::Search(KeyType key)
{
    Node* tmp = H;
    // start search from highest level
    for(int i = H->next.size()-1;i>=0;--i)
    {
        while(tmp->next[i] && tmp->next[i]->key < key)
            tmp = tmp->next[i];
    }   // after for loop, tmp should be exactly the pre of the searched key

    tmp = tmp->next[0];
    if(tmp && tmp->key == key)
        return tmp->val;
    // if not found, throw error message 
    throw runtime_error("Key Not Found!");
}

void SkipList::Update(KeyType key, ValueType val)
{
    // memo pre of all level, highest at begin
    // first do a search, memo all level pre in vector
    vector<Node*> pre;
    Node* tmp = H;
    for(int i = H->next.size()-1;i>=0;--i)
    {
        while(tmp->next[i] && tmp->next[i]->key < key)
            tmp = tmp->next[i];
        pre.push_back(tmp);
    }
    tmp = tmp->next[0]; // tmp is where we should do update

    if(tmp && tmp->key==key) tmp->val = val; // update val
    else    // need to generate a new node
    {
        // first gen the random level of new node 
        // with p = 1/2 to level up
        int level = 1;  // careful that here is the level_size(while the idx starts from 0)
        while(rand() > RAND_MAX>>1) ++level; 
        level = min(level,SIZE);        // do not need too many levels.

        // create a new node to insert
        Node* p = new Node; p->key=key; p->val=val;
        int min_size = min(level,(int)H->next.size());
        for(int i = 0;i<min_size;++i)
        {
            p->next.push_back(pre[pre.size()-1 - i] -> next[i]);
            pre[pre.size()-1 - i] -> next[i] = p;
        }

        // if level > current H->next.size(), then need adding p in H->next;
        int cHsize = H->next.size();
        for(int _=0;_<level - cHsize;++_)
        {
            H->next.push_back(p);
            p->next.push_back(nullptr);
        }
    }
}

bool SkipList::Delete(KeyType key)
{
    // first find where we should do delete
    vector<Node*> pre;
    Node* tmp = H;
    for(int i = H->next.size()-1;i>=0;--i)
    {
        while(tmp->next[i] && tmp->next[i]->key < key)
            tmp = tmp->next[i]; 
        pre.push_back(tmp);
    }
    tmp = tmp->next[0];

    if(tmp && tmp->key == key) // If Found key
    {
        // adjust ptr
        // in each level, it just like a singly-linked list
        for(int i = 0;i<pre.size();++i)
        {
            if(pre[pre.size()-1-i]->next[i] != tmp)
                break;
            pre[pre.size()-1-i]->next[i] = tmp->next[i];
        }
        // free node
        delete tmp;
        // update height
        while( H->next.back() == nullptr)
            H->next.pop_back(); 
       return true; 
    }
    return false;   // not found key
}

<<<<<<< HEAD
//                                  test code1
=======

// for testing
>>>>>>> 54340f6365e4a73524f8abb357ef91b6bca65598
int main(int argc, char const *argv[])
{
    SkipList L;
    vector<int> v{9,1,6,5,4,7,10,8,15};
    int cnt=0;
    for(auto x:v)
    {
        L.Update(x,cnt++);
    }
    cout<<L<<"\n";
    cout<<L.Search(10)<<"\n";
    L.Delete(10);
    cout << L << endl;
    // test the exception
    try
    {
        cout<<L.Search(10);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    // cout<<L<<"\n";
    // L.Delete(1);
    // cout<<L<<"\n";

    return 0;
}


//                                   test code2
/*
int main() {
    vector<int> v;
    SkipList L;
    int cnt = 0;
    srand(time(NULL));
    for(int n = 1; n <= 100000000; n *= 10) {
        double utime = 0, stime = 0, dtime = 0;
        clock_t ustart, uend, sstart, send, dstart, dend;
        for(int k = 0; k < n; k++) L.Update(rand(), cnt++);    // init the data scale
        for(int i = 0; i < 1000000; i++) {
            int a = rand();
            ustart = clock();
            L.Update(a, cnt++);
            uend = clock();
            sstart = clock();
            L.Search(a);
            send = clock();
            dstart = clock();
            L.Delete(a);
            dend = clock();
            utime += (uend-ustart);
            stime += (send-sstart);
            dtime += (dend-dstart);
        }
        freopen("update.txt", "a", stdout);
        cout << utime << endl;
        freopen("search.txt", "a", stdout);
        cout << stime << endl;
        freopen("delete.txt", "a", stdout);
        cout << dtime << endl;
    }
    return 0;
}
*/