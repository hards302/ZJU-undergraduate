#include<stdio.h>
#include<stdlib.h>

const int NUM=105;
const int IDNUM=1000;

typedef struct Book
{
    int* p;
    int psize;
    int price;
} Book;


// Solve = [size,book,dp,tips,prices](state,cpos,choice){}
// nums[i] 记录 [i,n-1] 中能取的 safe fruit 的最大数量
void Solve(int* state, int cpos, int choice, int size, Book* book,int* dp,int**tips,int* prices)
{
    state[cpos]=choice;
    // prune 1: use dp and size
    int count1 = 0;for(int i=0;i<=cpos;++i) count1+=state[i];
    if(count1+ size-cpos-1 < book->psize)return;
    if(cpos+1 < size && dp[cpos+1])
    {
        if(count1+dp[cpos+1]<book->psize) return;
    }

    if(cpos>=size-1)
    {
        // update book if necessary
        int* p = (int*)malloc(sizeof(int)*size);
        int psize = 0;
        int pprice = 0;
        for(int i=0,j=0;i<size;++i)
        {
            if(state[i])
            {
                p[j++]=i;
                psize++;
                pprice+=prices[i];
            }
        }
        if(psize>book->psize || (psize==book->psize && pprice<book->price))
        {
            if(book->p!=NULL)free(book->p);
            book->p = p;
            book->psize = psize;
            book->price = pprice;
        }
        return;
    }
    cpos++;
    Solve(state,cpos,0,size,book,dp,tips,prices);
    // when first came out, state should be 00000...i...
    // then we can update dp[cpos+1] use current book->psize
    if(cpos+1<size && !dp[cpos+1])
    {
        dp[cpos+1]=book->psize;
    }
    // test prune
    // prune 3: return immediately
    if(!dp[cpos])
    {
        int nconflict = 1;
        for(int i=cpos+1;i<size;++i)
        {
            if(state[i]&&*((int*)tips+i*NUM+cpos))
            {
                nconflict=0;
                break;
            }
        }
        if(nconflict)
        {
            dp[cpos]=dp[cpos+1]+1;  // remember to update dp
            state[cpos]=1;
            int* p = (int*)malloc(sizeof(int)*book->psize+1);
            int pprice=0;
            for(int i=0,j=0;i<size;++i)
            {
                if(state[i])
                {
                    p[j++]=i;
                    pprice+=prices[i];
                }
            }
            if(book->p!=NULL)free(book->p);
            book->p = p;
            book->psize++;
            book->price=pprice;
            return;
        }
    }
    
    // prune 2: use non-conflict
    int nconflict = 1;
    for(int i=0;i<cpos;++i)
    {
        if(state[i] && *((int*)tips+i*NUM+cpos))
        {
            nconflict=0;
            break;
        }
    }
    if(nconflict) Solve(state,cpos,1,size,book,dp,tips,prices);
}

int compare_ints(const void* a, const void *b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return (arg1>arg2) - (arg1<arg2);
}

int main(int argc, char const *argv[])
{
    freopen("./test.txt","r",stdin);
    int N,M;
    int names[NUM],prices[NUM];
    // idx 初始全部置为 -1，表示该 name 没有出现
    int idx[IDNUM];for(int i=0;i<IDNUM;++i)idx[i]=-1;
    int tips_tmp[NUM][2];   // 冲突点对暂存
    int tips[NUM][NUM];     // 冲突点邻接矩阵
    int dp[NUM];for(int i=0;i<NUM;++i)dp[i]=0;  // for memo
    int state[NUM];for(int i=0;i<NUM;++i)state[i]=0;    // cur_sol

    // 冲突邻接矩阵初始化
    for(int i=0;i<NUM;++i)
    {
        for(int j=0;j<NUM;++j)
        {
            tips[i][j]=0;
        }
    }
    // 读入数据
    scanf("%d%d",&N,&M);
    // 暂存冲突点对
    printf("edge number:%d point number:%d\n",N,M);
    for(int i=0;i<N;++i) scanf("%d%d",tips_tmp[i],tips_tmp[i]+1);
    // 建立 id->name,id->price, name->id
    for(int i=0;i<M;++i)
    {
        scanf("%d%d",names+i,prices+i);
        idx[names[i]]=i;
    }
    // 利用 idx, tips_tmp 建立冲突邻接矩阵
    for(int i=0;i<N;++i)
    {
        // 小心 id 不存在 对应 name 的情况，需排除
        int id1 = idx[tips_tmp[i][0]],id2 = idx[tips_tmp[i][1]];
        if(id1!=-1 && id2 !=-1)
        {
            tips[id1][id2] = 1;
            tips[id2][id1] = 1;
        }
    }
    Book book={NULL,0,0};
    int cpos=0;
    Solve(state,cpos,0,M,&book,dp,(int**)tips,prices);
    dp[cpos+1]=book.psize;
    Solve(state,cpos,1,M,&book,dp,(int**)tips,prices);
    // convert id to name
    for(int i=0;i<book.psize;++i)book.p[i] = names[book.p[i]];
    // sort by name
    qsort(book.p,book.psize,sizeof(int),compare_ints);
    // output res
    printf("%d\n%03d",book.psize,book.p[0]);
    for(int i = 1;i<book.psize;++i)
    {
        printf(" %03d",book.p[i]);
    }
    printf("\n%d\n",book.price);
    return 0;
}