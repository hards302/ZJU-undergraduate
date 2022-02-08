#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#include<queue>
using namespace std;
// def for BiNode for Huffman tree
class BiNode
{
    public:
        int val;
        BiNode* left;
        BiNode* right;
        BiNode(int _val=0){val=_val;left=nullptr;right=nullptr;}
};

const int MAX_N = 70;
char carr[MAX_N];
int frearr[MAX_N];
int N,M;

// implement the huffman coding on our own
int opt_num()
{
    auto cmp = [](const BiNode*a,const BiNode*b){return a->val>b->val;}; // use a cmp to init a priority_queue
    priority_queue<BiNode*,vector<BiNode*>,decltype(cmp)> bl(cmp);  // bl is the pq<BiNode*>, is a min_heap

    int res = 0;    // res = sum(fi*(1+di))
    BiNode nodes[MAX_N]; for(int i=0;i<N;++i)nodes[i].val = frearr[i];  // construct BiNodes
    for(int i = 0;i<N;++i)bl.push(nodes+i); // heapify
    // repeatedly pop 2 min and combine them ,and push the results 
    for(int i = 0;i<N-1;++i)
    {
        BiNode* z = new BiNode;
        z->left = bl.top(); bl.pop();
        z->right = bl.top();bl.pop();
        z->val = z->left->val+z->right->val;
        res+=z->val;        // remember to update res
        bl.push(z);
    }
    return res;
}

int main(int argc, char const *argv[])
{
    // input char and fre
    freopen("test.txt","r",stdin);
    // freopen("test.txt", "w", stdout);
    cin>>N;
    for(int i=0;i<N;++i) cin>>carr[i]>>frearr[i];
    // do huffman coding on our own to compute
    // the minimum of sum(fi*(1+di)), denote it optnum
    int optnum = opt_num();

    // check each students
    // first we check if sum(fi*(1+di)) is optnum
    // then we check if it is prefix code
    string stucodes[MAX_N]; // to store stu's code
    cin>>M;
    string tmp; // the c[i] is actually not need, we only need the code
    for(int stu=0;stu<M;++stu)
    {
        int stu_num = 0;
        for(int i=0;i<N;++i)
        {
            cin>>tmp>>stucodes[i];
            stu_num+= stucodes[i].size()*frearr[i]; // compute sum(fi*(1+di))
        }
        int flag = 1;   // print"Yes" if flag else print"No"
        if(stu_num!=optnum) flag = 0;   // not optimum code
        else
        {
            // if is optimum, then check if it is a prefix code
            // enumerate all possible pairs code[j] and code[k]
            // check if one is the prefix of another
            for(int j = 0;j<N;++j)
            {
                if(!flag) break;
                for(int k = j+1;k<N;++k)
                {
                    int tflag = 1; // if tflag then (one is prefix of another)
                    int min_size = stucodes[j].size() <= stucodes[k].size() ? stucodes[j].size():stucodes[k].size();
                    for(int id = 0;id< min_size;++id)
                    {
                        if(stucodes[j][id] != stucodes[k][id]){tflag = 0;break;}
                    }
                    if(tflag){flag = 0;break;}
                }
            }
        }
        if(flag) cout<<"Yes\n";
        else cout<<"No\n";
    }
    return 0;
}
