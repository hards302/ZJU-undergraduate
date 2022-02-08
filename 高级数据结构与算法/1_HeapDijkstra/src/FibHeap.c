#include"FibHeap.h"

// FNode constructor
FNode* FNode_Info(Info info)
{
    FNode* pnode = malloc(sizeof(FNode));
    pnode->info = info;
    pnode->child = NULL;pnode->parent = NULL;
    pnode->fsib = pnode;pnode->nsib = pnode;
    pnode->deg = 0; pnode->loser = 0;
    return pnode;
}
// FibHeap constructor
FibHeap* FibHeap_empty()
{
    FibHeap* H = malloc(sizeof(FibHeap));
    H->head = NULL;
    H->min = NULL;
}

// link FNode to rootlist (as a sib)
// need to set parent=NULL, loser=0
void link_node(Root *head, FNode* pnode)
{
    pnode->parent = NULL;
    pnode->loser = 0; 
    if(*head == NULL)
    {
        *head = pnode;
        pnode->fsib = pnode;
        pnode->nsib = pnode;
    }
    else
    {
        pnode->fsib = *head;
        pnode->nsib = (*head)->nsib;
        (*head)->nsib->fsib = pnode;
        (*head)->nsib = pnode;
    }
}

// delete FNode in rootlist
void delete_node(Root *head, FNode* pnode)
{
    pnode->nsib->fsib = pnode->fsib;
    pnode->fsib->nsib = pnode->nsib;
    if(pnode == *head) 
    {
        if(pnode->nsib == pnode)*head = NULL;
        else *head = (*head)->nsib;
    }
}

// swap link of the node
void swap_link(Root *p1,Root *p2)
{
    Root tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

// splice childlist to rootlist
void splice_list(FNode* rhead, FNode* chead)
{
    if(chead != NULL)
    {
        // set parent and loser of childlist
        FNode* tmp = chead;
        do
        {
            tmp->parent = NULL;
            tmp->loser = 0;
            tmp = tmp->nsib;
        } while (tmp!=chead);
        // splice childlist to rootlist
        rhead->fsib->nsib = chead;
        chead->fsib->nsib = rhead;
        swap_link(&(rhead->fsib),&(chead->fsib));
    }
}

// merge 2 root in root list
// return the new root
Root merge_root(Root r1, Root r2)
{
    if(r1->info.key < r2->info.key) // merge r2 -> r1
    {
        // delete r2 in root list
        //delete_node(h,r2);
        // link r2 to r1's child list
        r2->parent = r1;
        r1->deg++;
        if(r1->child == NULL) 
        {
            r1->child = r2;
            r2->nsib = r2;
            r2->fsib = r2;
        }
        else
        {
            r2->nsib = r1->child;
            r2->fsib = r1->child->fsib;
            r1->child->fsib->nsib = r2;
            r1->child->fsib = r2;
        }
        return r1;
    }
    else
    {
        // handle the sym situation
        return merge_root(r2,r1);
    }
}

void Insert(FibHeap *H, Info info,Root *map_heap)
{
    FNode* pnode = FNode_Info(info);
    // map name-> pnode
    map_heap[info.name] = pnode;
    // link node to root list directly
    link_node(&(H->head), pnode);
    // update minroot
    if(H->min == NULL || pnode->info.key < H->min->info.key)H->min = pnode;
}

Info Popmin(FibHeap* H, Root *map_heap)
{
    Info info = {-1,-1};
    if(H->head != NULL)
    {
        // splice H->min's child list with root list
        splice_list(H->head, H->min->child);
        // memo the info
        info = H->min->info;
        // delete H->min in map_heap
        map_heap[info.name] = NULL;
        // delete H->min in root list
        delete_node(&(H->head),H->min);
        // merge all trees with same deg
        const int NUM = 3000;
        Root aux[NUM]; for(int i=0;i<NUM;++i) aux[i]=NULL;
        Root tmp = H->head;
        // all result is stored in aux (H->head is actually destroyed)
        do
        {
            Root ntmp = tmp;
            tmp = tmp->nsib;
            while(aux[ntmp->deg]!=NULL)
            {
                int tmp_deg = ntmp->deg;
                ntmp = merge_root(ntmp,aux[tmp_deg]);
                aux[tmp_deg] = NULL;
            }
            aux[ntmp->deg] = ntmp;
        } while (tmp!=H->head);
        // copy all trees back to H->head, and update H->min
        H->head = NULL; H->min = NULL;
        for(int i = 0; i<NUM;++i)
        {
            if(aux[i] != NULL)
            {
                link_node(&(H->head),aux[i]);
                if(H->min==NULL || aux[i]->info.key < H->min->info.key)H->min = aux[i];
            }
        }
    }
    return info;
}

void Decrease_key(FibHeap* H, Info ninfo,Root *map_heap)
{
    // use map_heap find the handle pnode
    FNode* pnode = map_heap[ninfo.name];
    // decrease pnode's key
    pnode->info.key = ninfo.key;
    // update H->min
    if(ninfo.key < H->min->info.key)H->min = pnode;
    // if heap order violated
    if(pnode->parent != NULL && pnode->parent->info.key > pnode->info.key)
    {
        // memo pnode->parent
        FNode* pparent = pnode->parent;
        pparent->deg--; // will lose a child
        // 1. dump pnode to root list
        // delete pnode in child list
        delete_node(&(pparent->child),pnode);
        // link pnode to root list
        link_node(&(H->head),pnode);
        // 2. move up, dump any node to root list
        // until node is not a loser, then mark this node loser
        FNode* tmp = pparent;
        while(tmp->loser)
        {
            FNode* ntmp = tmp->parent;
            ntmp->deg--;    // will lose a child
            // delete pnode in child list
            delete_node(&(ntmp->child),tmp);
            // link tmp to root list
            link_node(&(H->head),tmp);
            // move tmp up
            tmp = ntmp;
        }
        if(tmp->parent != NULL) tmp->loser = 1;
    }
}