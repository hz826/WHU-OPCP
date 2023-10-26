#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <unistd.h>
using namespace std;

class Solution 
{
    public:
    int Role, StepID;
    int board[3][3];
    FILE *fp_log;

    int hx[5] = {0, 1, 1, 1};
    int hy[5] = {1, 0, 1, -1};

    int result[20000];
    struct BOARD{
        int map[3][3];
    };

    inline int encode(BOARD s)
    {
        int ans=0;
        for(int i=0; i<3; i++)
            for(int j=0;j<3;j++)
                ans = ans*3 + s.map[i][j];
        return ans;
    }

    inline BOARD decode(int S)
    {
        BOARD ans;
        for(int i=2; i>=0; i--)
            for(int j=2; j>=0; j--)
            {
                ans.map[i][j] = S % 3;
                S /= 3;
            }
        return ans;
    }

    struct node{
        int x, y;
        node() {}
        node(int A, int B) {x=A; y=B;}
    };

    int check(BOARD a)
    {
        for(int x=0; x<3; x++)
            for(int y=0; y<3; y++)
                for(int k=0;k<4;k++)
                {
                    int flag = 0;
                    for(int u=0;u<3;u++)
                    {
                        int tx = x + (u*hx[k]);
                        int ty = y + (u*hy[k]);
                        
                        if(tx<0 || ty<0 || tx>2 || ty>2 || !a.map[tx][ty])
                        {
                            flag = 0;
                            break;
                        }

                        if(!flag) flag = a.map[tx][ty];
                        if(flag != a.map[tx][ty])
                        {
                            flag = 0;
                            break;
                        }
                    }
                    if(flag) return flag;
                }
        return 0;
    }

    void dfs(int S, int op)
    {
        if(result[S]!=-1) return;
        BOARD a = decode(S);
        int tmp = check(a);
        if(tmp != 0)
        {
            result[S] = tmp;
            return;
        }
        
        int cnt=0;
        bool draw = false, win = false;

        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
                if(a.map[i][j] == 0)
                {
                    cnt++;
                    a.map[i][j] = op;
                    int T = encode(a);
                    dfs(T, 3-op);
                    a.map[i][j] = 0;

                    if(result[T] == 0) draw = true;
                    if(result[T] == op)
                    {
                        win = true;
                        break;
                    }
                }
            if(win) break;
        }
        
        if(!cnt) draw = true;
        if(win) result[S] = op;
        else if(draw) result[S] = 0;
        else result[S] = 3-op;
    }

    void init() 
    {
        srand(time(0));
        scanf("%d", &Role);
        StepID = 0;
    }

    void next_step() 
    {
        if (Role!=0 || StepID) 
        {
            int x, y;
            scanf("%d%d", &x, &y);
            board[x][y] = 2;
        }

        node Step;

        BOARD a;
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++) a.map[i][j] = board[i][j];
        
        if(!StepID)
        {
            memset(result, -1, sizeof(result));
            dfs(encode(a), 1);
        }

        ++StepID;

        bool win = false;
        for(int x=0; x<3; x++)
        {
            for(int y=0; y<3; y++)
                if(!a.map[x][y])
                {
                    a.map[x][y] = 1;
                    int T = encode(a);
                    a.map[x][y] = 0;

                    if(result[T] == 0) Step = node(x, y);
                    if(result[T] == 1)
                    {
                        Step = node(x, y);
                        win = true;
                        break;
                    }
                }
            if(win) break; 
        }
        
        a.map[Step.x][Step.y] = 1;

        board[Step.x][Step.y] = 1;
        printf("%d %d\n", Step.x, Step.y);
        fflush(stdout);

    }
} solotion;

int main() {
    solotion.init();
    while (1) 
    {
        solotion.next_step();
    }
}