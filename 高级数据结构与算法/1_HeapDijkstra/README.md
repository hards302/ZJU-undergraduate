## 1. 寻找最适宜 Dijkstra 算法的优先队列的具体实现
### Grading Policy
* Programming
    * Implement Dijkstra with Fib heap and other heaps
    * Write a test for performance
* Testing
    * Provide input files and give the run time table
    * Plot run_time - input_size for illustration
    * Write analysis and comments. (at least 30% commented)
* Documentation
    * Chap 1 : Intro. Problem description, purpose, and background of data structure and algorithms.
    * Chap 2 : DS/Algo Specification. Pseudo-code for all algo invloved; specification of main ds.
    * Overall report

### 要求
* Use [USA road network](http://www.dis.uniroma1.it/challenge9/download.shtml) for analysis
* At least 1000 pairs of query are required
* All the codes must be sufficiently commented

### 初步思路
可能的方案: 参考视频 [Advanced Heap and Analysis](https://www.bilibili.com/video/BV1Sh411S7Aw), [配套讲义](https://www.cl.cam.ac.uk/teaching/2021/Algorithms/notes2.pdf) 
1. 二叉堆（最简单的想法）
2. 二项队列
3. Fibonacci 堆（可能有最好的摊还时间界）

### 项目分工
#### 优先队列ADT 的接口
基本操作：
* PQ.insert(Element) (给入一个点(从1开始编号)以及从初始点到该点的权重) 
* PQ.popmin() (pop出一个pair类型，first指向点，second指向距离distance)
附加操作：
* PQ.update_key(Element) (**特别注意**：需要给到需更新key的优先队列中的元素的指针(引用)，否则无法实现在O(logN) 复杂度的更新key(找到那个元素就要 O(N))。 如果new_key小于old_key，则更新，否则不更新。)
