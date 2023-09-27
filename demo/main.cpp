#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string>
using std::string;

class Player {
    public:
    string compile_cmd, run_cmd, fifo_in, fifo_out;
    int fd_in, fd_out, pid;
    FILE *fp_in, *fp_out;

    Player(string compile_cmd, string run_cmd, string fifo_in, string fifo_out) 
        : compile_cmd(compile_cmd), run_cmd(run_cmd), fifo_in(fifo_in), fifo_out(fifo_out) {}

    void init() {
        unlink(fifo_in.c_str());
        unlink(fifo_out.c_str());
        if (mkfifo(fifo_in.c_str(),0777) < 0) { printf("cannot create fifo\n");  exit(1); } 
        if (mkfifo(fifo_out.c_str(),0777) < 0) { printf("cannot create fifo\n");  exit(1); }
        system(compile_cmd.c_str());
    }

    void run() {
        pid = fork();
        if (pid < 0) { printf("cannot fork\n");  exit(1); }
        if (pid == 0) {
            system((run_cmd + "<" + fifo_in + ">" + fifo_out).c_str());
            exit(0);
        }
        if ((fd_in = open(fifo_in.c_str(), O_WRONLY)) < 0) { printf("cannot open fifo\n");  exit(1); }
        if ((fd_out = open(fifo_out.c_str(), O_RDONLY)) < 0) { printf("cannot open fifo\n");  exit(1); }

        if ((fp_in = fdopen(fd_in, "w")) == NULL) { printf("cannot fdopen fifo\n");  exit(1); }
        if ((fp_out = fdopen(fd_out, "r")) == NULL) { printf("cannot fdopen fifo\n");  exit(1); }
    }

    void resetTimer() {

    }

    void signal_stop() {
        kill(pid, SIGSTOP);
    }

    void signal_continue() {
        kill(pid, SIGCONT);
    }
};

int board[3][3];
void endgame(int x) {
    printf("END: %d\n", x);
    exit(0);
}

void add(int turn, int x, int y) {
    char c = (turn == 1) ? 'A' : 'B';
    printf("%c: %d %d\n", c, x, y);
    if (x < 0 || x > 2 || y < 0 || y > 2 || board[x][y] != 0) {
        printf("%c: invalid operation\n", c);
        endgame((turn == 1) ? -1 : 1);
    }
    board[x][y] = turn;
    const char ch[3] = {' ', 'o', 'x'};
    puts("");
    for (int i=0;i<3;i++) {
        for (int j=0;j<3;j++) putchar(ch[board[i][j]]);
        putchar('\n');
    }

    if ((board[x][0] == turn && board[x][1] == turn && board[x][2] == turn) ||
        (board[0][y] == turn && board[1][y] == turn && board[2][y] == turn) ||
        (board[0][0] == turn && board[1][1] == turn && board[2][2] == turn) ||
        (board[0][2] == turn && board[1][1] == turn && board[2][0] == turn)) {
            endgame((turn == 1) ? 1 : -1);
        }
    
    int counter = 0;
    for (int i=0;i<3;i++) for (int j=0;j<3;j++) if (board[i][j] == 0) counter++;
    if (!counter) endgame(0);
}

int main(int argc, char *argv[]) {
    Player Alice("g++ A.cpp -o A", "./A", "fifo_CA", "fifo_AC");
    Player Bob("g++ B.cpp -o B", "./B", "fifo_CB", "fifo_BC");
    
    Alice.init();
    Bob.init();
    printf("init finished\n");

    int x, y;
    memset(board, 0, sizeof(board));

    Alice.run();
    fprintf(Alice.fp_in, "%d\n", 1);  fflush(Alice.fp_in);
    fscanf(Alice.fp_out, "%d%d", &x, &y); // timeout
    
    add(1, x, y);

    Bob.run();
    fprintf(Bob.fp_in, "%d\n%d %d\n", 2, x, y);  fflush(Bob.fp_in);
    fscanf(Bob.fp_out, "%d%d", &x, &y); // timeout
    add(2, x, y);

    int turn = 1;
    while (1) {
        Player &player = (turn == 1 ? Alice : Bob);
        fprintf(player.fp_in, "%d %d\n", x, y);  fflush(player.fp_in);
        fscanf(player.fp_out, "%d%d", &x, &y); // timeout
        add(turn, x, y);
        turn ^= 3;
    }
}