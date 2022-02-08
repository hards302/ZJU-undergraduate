#include<stdio.h>
#include<stdlib.h>

const int NUM=105;
const int IDNUM=1000;

// for memo the final res
typedef struct Book
{
    int* p;
    int psize;
    int price;
} Book;

// cmp func for qsort
int compare_ints(const void* a, const void *b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return (arg1>arg2) - (arg1<arg2);
}

/*
recurion func Solve
void [size,book,rfind,tips,prices] Solve(state, cpos,choice) 

FUNCTION's CLOSURE(which act like a global var specially for this recur func, so we pass it by ref)
    size    # of fruits
    book    memo of final res
    rfind   rfind[i] means the max # of safe fruit you can find in [i,...,N-1]
            e.g. Suppose N-1, and N-2 are conflict, but both are nonconflict with N-3
                then rfind[N-1] = 1, rfind[N-2] = 1, rfind[N-3] = 2, and we set rfind[N]=0
    rfind is crucial for implement pruning
    tips    the collision adjacent matrix
    prices  prices[i] is the price of fruit i

FUNCTION's current INPUT
    state   current situation, based on which recur func make decision(return, memo the res, or go deeper)
    cpos    current position
    choice  how to update our current state
*/
void Solve(int* state, int cpos, int choice, int size, Book* book,int* rfind,int**tips,int* prices);

int main(int argc, char const *argv[])
{
    /*
    SET UP all the Variable we need
    we use fruit's id(for the first fruit occur in input, we give it id 0)
    (only 100 fruits) as index, not name(can vary from 1-999)
    in this way we can speed up and save space
    So we need to set up names and idx
    to build one-one map between name and id
    */
    int N,M;    // N: # of collision pairs; M: # of fruits
    int names[NUM],prices[NUM]; // id->name; id->price
    int idx[IDNUM];for(int i=0;i<IDNUM;++i)idx[i]=-1;   //name->id;idx[i] init as -1, repr this name not occur in input
    int tips_tmp[NUM][2];   // save conflict pair(name based)
    int tips[NUM][NUM];     // use adjacent matrix to speed up search(id based, so need conversion)
    int rfind[NUM];for(int i=0;i<NUM;++i)rfind[i]=0;  // init rfind(check rfind def ahead Solve func if not clear) 
    int state[NUM];for(int i=0;i<NUM;++i)state[i]=0;    // state means

    // Init tips; if tips[i][j]==1, then id_i fruit is conflict with id_j fruit
    for(int i=0;i<NUM;++i)
    {
        for(int j=0;j<NUM;++j)
        {
            tips[i][j]=0;
        }
    }
    
    // INPUT START
    scanf("%d%d",&N,&M);
    // store the conflict pairs in tips_tmp
    for(int i=0;i<N;++i) scanf("%d%d",tips_tmp[i],tips_tmp[i]+1);
    // Establish id->name,id->price, name->id
    for(int i=0;i<M;++i)
    {
        scanf("%d%d",names+i,prices+i);
        idx[names[i]]=i;
    }
    // build tips based on tips_tmp and idx
    for(int i=0;i<N;++i)
    {
        // watch out the situation that id does not have a corresponding name
        // we need to exclude it 
        int id1 = idx[tips_tmp[i][0]],id2 = idx[tips_tmp[i][1]];
        if(id1!=-1 && id2 !=-1)
        {
            tips[id1][id2] = 1;
            tips[id2][id1] = 1;
        }
    }
    // INPUT END
    Book book={NULL,0,0};// to store res
    int cpos=0;     // we start from 0
    // set state[0]=0;
    Solve(state,cpos,0,M,&book,rfind,(int**)tips,prices);
    rfind[cpos+1]=book.psize;   // now is 0{1,...N} state, so we need to update rfind[1]
    #ifndef NO_PRUNE_C
    int nconflict=1;
    for(int i=0;i<book.psize;++i)
    {
        if(tips[cpos][book.p[i]])
        {
            nconflict=0;
            break;
        }
    }
    if(nconflict)
    {
        // update book
        int* p = (int*)malloc(sizeof(int)*book.psize+1);
        p[0] = cpos;
        book.price+=prices[cpos];
        for(int i=0;i<book.psize;++i)
        {
            p[i+1] = book.p[i];
        }
        if(book.p != NULL) free(book.p);
        book.p = p;
        book.psize++;
        rfind[cpos] = rfind[cpos+1]+1;
    }
    else 
    #endif
    Solve(state,cpos,1,M,&book,rfind,(int**)tips,prices);
    
    // OUTPUT START
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
    // OUTPUT END
    return 0;
}

void Solve(int* state, int cpos, int choice, int size, Book* book,int* rfind,int**tips,int* prices)
{
    // update(state,cpos)
    state[cpos]=choice;
    // count 1s in state[0,cpos]
    int count1 = 0;for(int i=0;i<=cpos;++i) count1+=state[i];
    #ifndef NO_PRUNE_A  // for testing PRUNE_A's effect
    /*
        PRUNE_A
        suppose state[cpos,size-1] is all 1
        and still can't be better than book->psize
        then we don't need to go deeper anymore, return right now
    */
    if(count1+ size-cpos-1 < book->psize)return;
    #endif
    #ifndef NO_PRUNE_B  
    /*
        PRUNE_B(somehow like PRUNE_A)
        if now we know rfind[cpos+1]
        and we know rfind[cpos+1] is the best possible reward if we choose to go deeper
        and still can't be better than book->psize
        then we should return right now
    */
    if(rfind[cpos+1] && count1+rfind[cpos+1]<book->psize) return;
    #endif

    // now if we hit the end, then we should check if our answer is better
    if(cpos>=size-1)
    {
        // update book if necessary
        int* p = (int*)malloc(sizeof(int)*size);// mem cur_sol's id
        int psize = 0;  // mem the size
        int pprice = 0; // mem the price
        for(int i=0,j=0;i<size;++i)
        {
            if(state[i])
            {
                p[j++]=i;
                psize++;
                pprice+=prices[i];
            }
        }
        // if size is bigger or (size is the same,but cheaper)
        // we need to update it
        if(psize>book->psize || (pprice<book->price && psize==book->psize))
        {
            if(book->p!=NULL)free(book->p); // prevent mem leakage
            book->p = p;
            book->psize = psize;
            book->price = pprice;
        }
        // then we should return
        return;
    }
    
    // Now we go to next
    cpos++;
    // it is always legitimate not to put the next fruit in the safe busket
    Solve(state,cpos,0,size,book,rfind,tips,prices);
    /*
        UPDATE rfind
        when first came out, state should be 00000...i...
        then we can update rfind[cpos+1] use current book->psize
        and note that rfind[i]>0,for any i in [0,N-1]
        so if !rfind[i], it has not been updated
    */
    if(cpos+1<size && !rfind[cpos+1])rfind[cpos+1]=book->psize;

    /*
        PRUNE_C
        only apply to the situation that rfind[cpos]==0
        which means right now is 00...0[cpos]....
        and we are now deciding whether to go deeper into gametree of state[cpos]==1

        note that rfind[num] = rfind[num+1] or rfind[num+1]+1
        and rfind[num] = rfind[num+1]+1 
        if and only if num is not conflict with all state[cpos+1,size-1]==1

        so if we know that num is not conflict with all state[cpos+1,size-1]==1
        then we can deduce the best of the gametree is to put cpos in right now
        which means we can know the answer without going deeper

        so we can update the answer if necessary and return now
    */
    #ifndef NO_PRUNE_C
    if(!rfind[cpos])
    {
        // check if conflict with book->p
        int nconflict = 1;
        for(int i=0;i<book->psize;++i)
        {
            if(*((int*)tips+cpos*NUM+book->p[i]))
            {
                nconflict=0;
                break;
            }
        }
        if(nconflict)
        {
            // update book
            int* p = (int*)malloc(sizeof(int)*book->psize+1);
            p[0] = cpos;
            book->price+=prices[cpos];
            for(int i=0;i<book->psize;++i)p[i+1] = book->p[i];
            if(book->p != NULL) free(book->p);
            book->p = p;
            book->psize++;
            rfind[cpos] = rfind[cpos+1]+1;
            return;
        }
    }
    #endif
    
    // if cpos is confict with state[0,cpos-1]==1
    // then we can't put it inside, we need to return right now
    int nconflict = 1;
    for(int i=0;i<cpos;++i)
    {
        if(state[i] && *((int*)tips+i*NUM+cpos))
        {
            nconflict=0;
            break;
        }
    }
    if(!nconflict) return;

    // if everything is fine and we can't do optimization
    // then we should go deeper to find the answer
    Solve(state,cpos,1,size,book,rfind,tips,prices);
}
