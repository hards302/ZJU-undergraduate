# README.md

## SkipList API and examples

```C++
SkipList L;	// init as an empty skiplist

// overload <<, you can print it easily
cout<<L<<"\n";

// search a key; if key not exist, throw an error
try
	ValueType val = L.Search(key);
catch(exception& s)
    cerr<<e.what()<<"\n";

// update a key(if not exsit, then insert it)
L.Update(key,val);

// delete a key-val pair; if key not exist, return false
if( L.Delete(key) )
    // delete the existed key
else
    // not found key; have done nothing
```

## Data Structure

Generally, skip list is used to implement dictionary, which stores key-value pair and is a dynamic data structure supporting fast search.

We use the Node structure as the node of link list. Everything is like a single-linked-list, with only the difference the next pointers are a vector.

```C++
struct Node
{
    KeyType key;
    ValueType val;
    vector<Node*> next;    
}
```

Then we can have our SkipList class

```c++
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
```



