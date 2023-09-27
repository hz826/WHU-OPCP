#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

class Solution {
    public:
    int Role, StepID;
    int board[3][3];

    void init() {
        srand(time(0));
        scanf("%d", &Role);
        StepID = 0;
    }

    void next_step() {
        if (Role!=1 || StepID) {
            int x, y;
            scanf("%d%d", &x, &y);
            board[x][y] = 2;
        }

        ++StepID;
        int u[3] = {0,1,2}, v[3] = {0,1,2};
        random_shuffle(u, u+3);
        random_shuffle(v, v+3);

        int xx, yy;
        for (int i=0;i<3;i++) for (int j=0;j<3;j++) if (!board[u[i]][v[j]]) {
            xx = u[i];
            yy = v[j];
            break;
        }
        board[xx][yy] = 1;

        printf("%d %d\n", xx, yy);
        fflush(stdout);
    }
} solotion;

int main() {
    solotion.init();
    while (1) {
        solotion.next_step();
    }
}