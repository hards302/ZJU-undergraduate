import math
import copy
import random
class AIPlayer:
    """
    AI 玩家
    """

    def __init__(self, color):
        """
        玩家初始化
        :param color: 下棋方，'X' - 黑棋，'O' - 白棋
        """

        self.color = color
        
    def ucb1(self, node, t, c):
        '''
        计算ucb1值
        :param node: 蒙特卡洛树的节点
        :param t: 总搜索的次数
        :param c: ucb1函数计算中的常数c
        :return: ucb1的值
        '''
        pos, T, reward, childern = node # pos即该node的落子位置，T表示t次摇动中经历node的次数，reward即该node的奖励
        if T == 0:
            T = 0.000000001
        if t == 0:
            t = 1
        a = reward / T
        b = math.log(t)
        d = c * math.sqrt(2 * b / T)
        return a + d
    
    def select_path(self, root, t):
        '''
        蒙特卡洛树搜索第一步，选择
        :param root: 蒙特卡洛树根节点
        :param t: 摇动次数
        :return: 从根节点走到目前选定叶子节点的node集合tuple
        '''
        current_path = []
        childern = root
        temp_t = t
        AI_turn = True
        
        while True:
            if len(childern) == 0:
                break
            maxchild = []           # 记录ucb1值最大的孩子。可能存在相同的情况，所以用list保存
            childid = 0             # 孩子的下标
            
            if AI_turn:
                maxval = -1         # 记录ucb1值的最大/最小值
            else:
                maxval = 2
            
            for child in childern:
                pos, times, reward, t_childern = child
                if AI_turn:
                    cval = AIPlayer.ucb1(self, child, temp_t, 1)
                    if cval >= maxval:
                        if cval == maxval:
                            maxchild.append(childid)
                        else:
                            maxchild = [childid]
                            maxval = cval
                else:
                    cval = AIPlayer.ucb1(self, child, temp_t, -1)
                    if cval <= maxval:
                        if cval == maxval:
                            maxchild.append(childid)
                        else:
                            maxchild = [childid]
                            maxval = cval
                            
                childid = childid + 1
                
            maxid = maxchild[random.randrange(0, len(maxchild))]  # 随机在最大孩子间选一个
            pos, times, reward, t_childern = childern[maxid]
            current_path.append(pos)
            temp_t = times
            childern = t_childern
            AI_turn = not(AI_turn)
            
        return current_path
    
    def expand(self, board, color):
        '''
        蒙特卡洛树搜索第二步，扩展
        :param board: 棋盘
        :param color: 将要扩展的棋子的颜色，X-黑棋，O-白棋
        :return: 生成合法的落子坐标
        '''
        pos = list(board.get_legal_actions(color))
        childern = []
        for p in pos:
            childern.append((p, 0, 0, []))
        return childern
        
        
    def simulation(self, board, color):
        '''
        蒙特卡洛树搜索第三步，模拟
        :param board: 棋盘
        :param color: 将要落子的颜色
        :param steps: 模拟的步数
        :return: 是否可以赢
        '''
        
        # 接下来进行随机落子，一人一步
        action_list = list(board.get_legal_actions(color))
        
        if len(action_list) == 0:
            if color == 'X':
                neg_color = 'O'
            else:
                neg_color = 'X'
            neg_action_list = list(board.get_legal_actions(neg_color)) # 如果当前行动方没有落子的地方，则继续对手落子
            if len(neg_action_list) == 0:
                return board.get_winner()
            action_list = neg_action_list
            color = neg_color
            
        action = random.choice(action_list)
        board._move(action, color)
        
        color = 'X' if color == 'O' else 'O'
        return AIPlayer.simulation(self, board, color) # 递归进行随机下棋直到结束
        
    def back_propagation(self, board, root, iswon, path, color):
        '''
        蒙特卡洛树搜索第四步，回溯
        :param board: 棋盘
        :param root: 蒙特卡洛树的根节点
        :param iswon: 第三步中是否赢了
        :param path: 回溯的node路径
        :param color: 当前下棋的颜色
        '''
        childern = root
        for maxele in path:
            cid = 0           # 孩子的下标
            for child in childern:
                pos, times, reward, t_childern = child
                if maxele == pos:
                    break
                cid = cid + 1
                
            if maxele == pos:
                times += 1
                if iswon:
                    reward += 1
                if len(t_childern) == 0: # 如果遇到叶子节点，则向下扩展一层孩子
                    t_childern = AIPlayer.expand(self, board, color)
                childern[cid] = (pos, times, reward, t_childern)
            childern = t_childern
        
        return root

    def get_move(self, board):
        """
        根据当前棋盘状态获取最佳落子位置
        :param board: 棋盘
        :return: action 最佳落子位置, e.g. 'A1'
        """
        if self.color == 'X':
            player_name = '黑棋'
        else:
            player_name = '白棋'
        print("请等一会，对方 {}-{} 正在思考中...".format(player_name, self.color))

        # -----------------请实现你的算法代码--------------------------------------
        action = None
        temp_board1 = copy.deepcopy(board) # 深拷贝，防止修改棋盘
        temp_board2 = copy.deepcopy(board)
        root = AIPlayer.expand(self, temp_board1, self.color)
#         print(f'root = {root}')
        for ep in range(1, 5000):
            path = AIPlayer.select_path(self, root, ep) 
            # print(f'path = {path}')
            
            temp_color = self.color
            for pos in path:
                temp_board1._move(pos, temp_color)
                temp_color = 'X' if temp_color == 'O' else 'O'
            
            isWon = AIPlayer.simulation(self, temp_board2, self.color)
            root = AIPlayer.back_propagation(self, temp_board1, root, isWon, path, self.color)
        
        max_reward = -1
#         print(f'root = {root}')
        for child in root:
            pos, times, reward, t_childern = child
            if times > 0 and reward / times > max_reward:
                action = pos
                max_reward = reward / times
#         print(f'action = {action}')
        # ------------------------------------------------------------------------

        return action
