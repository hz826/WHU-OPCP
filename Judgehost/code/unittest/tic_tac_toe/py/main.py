from random import choice

class Solution :
    def init(self) :
        self.Role = int(input())
        self.StepID = 0
        self.board = [[0 for _ in range(3)] for _ in range(3)]
    
    def next_step(self) :
        if self.Role != 0 or self.StepID != 0 :
            (x,y) = tuple(map(int, input().split()))
            self.board[x][y] = 2

        self.StepID += 1
        (xx,yy) = choice([(i,j) for i in range(3) for j in range(3) if self.board[i][j] == 0])
        self.board[xx][yy] = 1
        print(xx, yy, flush=True)

solution = Solution()
solution.init()
while True :
    solution.next_step()