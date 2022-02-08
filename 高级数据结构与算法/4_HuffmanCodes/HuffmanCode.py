import heapq
import sys
# 重定向
fin = open("test.txt","r")
sys.stdin = fin

# binary tree
class BiNode:
    def __init__(self,val=0):
        self.fre = val
        self.left = None
        self.right = None
    def __le__(self, other):
        return self.fre<=other.fre
    def __lt__(self,other):
        return self.fre<other.fre

N = int(input())
l = input().split()
l = [l[0::2],list(map(int,l[1::2]))]
bl = [BiNode(i) for i in l[1]]
heapq.heapify(bl)
opt_num=0
for i in range(N-1):
    z = BiNode()
    z.left = heapq.heappop(bl)
    z.right = heapq.heappop(bl)
    z.fre = z.left.fre+z.right.fre
    opt_num+=z.fre
    heapq.heappush(bl,z)
M = int(input())
for i in range(M):
    tl = [input().split() for i in range(N)]
    sl = [i[1] for i in tl]
    st_num = sum(len(sl[i])*l[1][i] for i in range(N))
    flag = True
    if st_num!=opt_num:
        flag = False
    else:
        ssl = sorted(sl,key=lambda x:len(x))
        for i in range(len(ssl)):
            if not flag:
                break
            for j in range(i+1,len(ssl)):
                if ssl[i] == ssl[j][:len(ssl[i])]:
                    flag = False
                    break
    if flag:
        print("Yes")
    else:
        print("No")










