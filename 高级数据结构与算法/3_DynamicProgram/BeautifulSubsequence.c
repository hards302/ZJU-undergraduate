#include <stdio.h>
#include <math.h>
#define MAX_N 100000	
#define MO 1000000007
#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))
inline long long Rightest1(long long index);
long long GetSum(long long left, long long right, long long* FwTree);		// get sum of [left, right] 
void UpdateTree(long long index, long long x, long long* FwTree);			// update the tree
long long SumFromBegin(long long index, long long* FwTree);					// get sum of [1, index] 

int main(void) 
{
	long long n, diff;
	scanf("%lld %lld", &n, &diff);
	long long element;		// the element in the original sequence
	long long cur_bsn = 0;	// current number of beautiful sequences  

	long long FwTree1[MAX_N + 5] = { 0 };
	long long FwTree2[MAX_N + 5] = { 0 };
	//有两个Fenwick tree  各数组索引均自1起
	//FwTree[0][]存 “一个元素被考虑时的bsn”				服务于第1种情况 
	//FwTree[1][]存 “一个元素的2^i”（i指该元素第i个读入）	服务于第2种情况

	// initialize the tree2
	scanf("%lld", &element);
	UpdateTree(element, pow(2, 0), FwTree2);

	for (long long i = 1; i < n; i++) {
		scanf("%lld", &element);
		// 按element+/-diff将已读入的数分成两类 进行两种处理 (加它被考虑时的bsn数 或 加2^i) 
		// 原本是遍历已读入的数 并用if分支进行两种处理 现在把已读未读的数都按index置于数组处 
		// 此时还未读入的数处为0 无影响
		// 1. 对于那些与element差距大的已读入数 加它们被考虑时的bsn
		long long update1 = (
			GetSum( min(element+diff+1, MAX_N), MAX_N, FwTree1) % MO
			+ GetSum(1, max(element-diff-1, 1), FwTree1) % MO
			) % MO;
		// 2. 对于那些与element差距小的已读入数 加2^i (它是第i个读入的)
		long long update2 = ( GetSum( max(element-diff, 1), min(element+diff, MAX_N), FwTree2)) % MO;
		// 合并得到 为了得到当前bsn 应加的值
		long long update = (update1 + update2) % MO;
		cur_bsn = (cur_bsn + update) % MO;		//update current number of beautiful sequences

		UpdateTree(element, update, FwTree1);		//update tree1
		UpdateTree(element, pow(2, i), FwTree2);	//update tree2
	}
	printf("%lld", cur_bsn);
	return 0;
}

long long GetSum(long long left, long long right, long long* FwTree)	//get sum of [left, right] 
{
	return(SumFromBegin(right, FwTree) - SumFromBegin(left - 1, FwTree));
}
void UpdateTree(long long index, long long x, long long* FwTree)	//更新(默认为+x)
{
	//更新 从根到index叶节点这条路上的所有点
	while (index < MAX_N) {
		FwTree[index] = (FwTree[index] + x) % MO;
		index += Rightest1(index);	//index自增Rightest1的值后 即为父节点
	}
}
long long SumFromBegin(long long index, long long* FwTree)		//get sum of [1, index]
{
	long long sum = 0;
	while (index > 0) {
		sum = (sum + FwTree[index]) % MO;
		index -= Rightest1(index);	//index自减Rightest1的值后 即为为了求该区间和 还要加的树节点
	}
	return sum;
}
long long Rightest1(long long index)
{
	return(index&(-index));
} //只将最右的1保留 其它置0 的值
