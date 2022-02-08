#include"FibonacciHeap.h"


// CONSTRUCTORS
// construct deg0 Root from double
void Node_construct(Node* pnode, KeyType value)
{
    pnode->key = value;
    pnode->lchild = NULL;
    pnode->nsib = NULL;
    pnode->parent = NULL;
    pnode->deg = 0;
    pnode->loser = 0;
}

// construct listnode from root
void LNode_construct(LNode* plnode, Root root)
{
    plnode->p = root;
    root->parent = NULL;
    root->loser = 0;
    plnode->next = plnode;
    plnode->forward = plnode;
}

// construct an empty FibHeap
void FibHeap_construct(FibHeap* H)
{
    H->head = NULL;
    H->minroot = NULL;
}

// utility function
void link_infront(LNode* *plnode, LNode* *head)
{
    if(*head != NULL)
    {
        (*plnode)->next = *head;
        (*plnode)->forward = (*head)->forward;
        (*plnode)->forward->next = (*plnode);
        (*plnode)->next->forward = (*plnode);
    }
    else
    {
        (*plnode)->next = (*plnode);
        (*plnode)->forward = (*plnode);
    }
    *head = *plnode;
}

void delete_lnode(LNode* plnode, LNode* *head)
{
    plnode->forward->next = plnode->next;
    plnode->next->forward = plnode->forward;
    if(plnode == *head) 
    {
        if(plnode->next == plnode) *head = NULL;
        else *head = plnode->next;
    }
}

// Lazy Insertion
void Insert(FibHeap* H, double value)
{
    // construct a new deg0 tree(pnode) using value
    Root pnode = malloc(sizeof(Node));
    Node_construct(pnode,value);
    // construct a new list element(plnode) using pnode
    LNode* plnode = malloc(sizeof(LNode));
    LNode_construct(plnode,pnode);
    // link plnode in the front of H->head and update H->head;
    link_infront(&plnode,&(H->head));
    // update the minroot if necessary
    if(H->minroot==NULL || value < H->minroot->p->key)
    {
        H->minroot = plnode;
    }
}

// Popmin
// when combined, it should pop a handle for graph side
void Popmin(FibHeap* H)
{
    // find minroot
    LNode* minroot = H->minroot;
    if(minroot==NULL) return;
    else
    {
        // insert minroot's child before H->head, and update H->head
        if(minroot->p->lchild != NULL)
        {
            Node* tmp = minroot->p->lchild;
            do
            {
                LNode* plnode = malloc(sizeof(LNode));
                LNode_construct(plnode,tmp);
                link_infront(&plnode,&(H->head));
                tmp = tmp->nsib;
            }while(tmp!=NULL);
        }
        // delete minroot
        delete_lnode(minroot,&(H->head));
    }
    // merge all trees with same deg
    // create a auxillary arr with size logN
    const int NUM = 2000;
    Root aux[NUM]; for(int i=0;i<NUM;++i) aux[i]=NULL;
    // 遍历双向链表
    if(H->head == NULL) minroot=NULL;
    else
    {
        LNode* tmp = H->head;
        do
        {
            Root ntmp = tmp->p;
            // 利用 aux 辅助归并
            while(aux[ntmp->deg] != NULL)
            {
                if(aux[ntmp->deg]->key > ntmp->key)
                {
                    aux[ntmp->deg]->nsib = ntmp->lchild;
                    aux[ntmp->deg]->parent = ntmp;
                    ntmp->lchild = aux[ntmp->deg];
                    ntmp->deg++;
                }
                else
                {
                    ntmp->nsib = aux[ntmp->deg]->lchild;
                    ntmp->parent = aux[ntmp->deg];
                    aux[ntmp->deg]->lchild = ntmp;
                    aux[ntmp->deg]->deg++;
                    ntmp = aux[ntmp->deg];
                }
                aux[ntmp->deg-1] = NULL;
            }
            aux[ntmp->deg] = ntmp;
            tmp = tmp->next;
        }while(tmp!=H->head);
        // 将 aux 中结果 copy 到 list 中
        // 首先 list 就置空，然后再复制进去（是内存安全的）
        H->head = NULL;
        for(int i = 0; i<NUM;++i)
        {
            if(aux[i] != NULL)
            {
                LNode* plnode = malloc(sizeof(LNode));
                LNode_construct(plnode, aux[i]);
                link_infront(&plnode, &(H->head));
            }
        }
        // update minroot
        tmp = H->head;
        KeyType min_val = tmp->p->key;
        H->minroot = tmp;
        while (tmp->next != H->head)
        {
            if(tmp->p->key < min_val)
            {
                min_val = tmp->p->key;
                H->minroot = tmp;
            }
            tmp = tmp->next;
        }
    }
    return;
}

Node* dump_rlist(FibHeap* H, Node* pnode)
{
    // delete pnode from its parent
    Node* tmp = pnode->parent->lchild;
    if(tmp == pnode)pnode->parent->lchild = pnode->nsib;
    else
    {
        // find pre of pnode
        while(tmp->nsib!=pnode) tmp = tmp->nsib;
        tmp->nsib = pnode->nsib;
    }
    // booking the parent, and return it
    tmp = pnode->parent;
    // splice pnode to root list
    LNode* plnode = malloc(sizeof(LNode));
    LNode_construct(plnode,pnode);
    link_infront(&plnode,&(H->head));
}

// Update_key
// and we only decrease key
void Update_key(FibHeap* H, Node* pnode, KeyType nvalue)
{
    pnode->key = nvalue;
    if(pnode->parent != NULL && nvalue < pnode->parent->key)
    {
        Node* tmp = dump_rlist(H,pnode);
        // splice all the loser node
        // rule : lose 1 child, you are loser;
        //        lose 2, yourself get dumped to root list
        Node* ntmp = tmp;
        while(ntmp->loser)
        {
            ntmp = dump_rlist(H,ntmp);
        }
    }
    // update minroot
    
}
