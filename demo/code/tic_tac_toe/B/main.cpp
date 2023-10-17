#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <unistd.h>
using namespace std;

#define LOG

class Solution {
    public:
    int Role, StepID;
    int board[3][3];
    FILE *fp_log;

    void init() {
        #ifdef LOG
        fp_log = fopen("log.txt", "w");
        fprintf(fp_log, "init\n");
        fflush(fp_log);
        #endif

        srand(time(0));
        scanf("%d", &Role);
        StepID = 0;

        #ifdef LOG
        fprintf(fp_log, "role : %d\n", Role);
        fflush(fp_log);
        #endif
    }

    void next_step() {
        // for (int i=0;i<=1e9;i++) ;

        if (Role!=0 || StepID) {
            int x, y;
            scanf("%d%d", &x, &y);
            board[x][y] = 2;
            #ifdef LOG
            fprintf(fp_log, "input : %d %d\n", x, y);
            fflush(fp_log);
            #endif
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

        #ifdef LOG
        fprintf(fp_log, "output : %d %d\n", xx, yy);
        fflush(fp_log);
        #endif
    }
} solotion;

int main() {
    solotion.init();
    while (1) {
        solotion.next_step();
    }
}