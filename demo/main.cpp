#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

FILE *details, *score;

class Player;
vector<Player> players;
int player_cnt = 0;

class Player {
    public:
    int player_id, score;
    string compile_cmd, run_cmd, fifo_in, fifo_out;
    int fd_in, fd_out, pid;
    FILE *fp_in, *fp_out;

    Player(string compile_cmd, string run_cmd, string fifo_in, string fifo_out) 
        : compile_cmd(compile_cmd), run_cmd(run_cmd), fifo_in(fifo_in), fifo_out(fifo_out) {
            player_id = player_cnt++;
        }

    void init() {
        unlink(fifo_in.c_str());
        unlink(fifo_out.c_str());
        if (mkfifo(fifo_in.c_str(),0777) < 0) { printf("cannot create fifo\n");  exit(1); } 
        if (mkfifo(fifo_out.c_str(),0777) < 0) { printf("cannot create fifo\n");  exit(1); }
        system(compile_cmd.c_str());
    }

    void clean() {
        free(fp_in);  free(fp_out);
        close(fd_in);  close(fd_out);
        unlink(fifo_in.c_str());
        unlink(fifo_out.c_str());
        signal_kill();
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

    void setTimeout(int T, bool resetTimer) {
        
    }

    void checkTimeout() {

    }

    void signal_stop() {
        kill(pid, SIGSTOP);
    }

    void signal_continue() {
        kill(pid, SIGCONT);
    }

    void signal_kill() {
        kill(pid, SIGKILL);
    }

    template<typename... Args>
    void _scanf(Args... args) {
        fscanf(fp_out, args...);
    }

    template<typename... Args>
    void _printf(Args... args) {
        fprintf(fp_in, args...);
        fflush(fp_in);
    }

    template<typename... Args>
    void _log(Args... args) {
        fprintf(details, "%c: ", 'A'+player_id);
        fprintf(details, args...);
    }
};

void endgame(int winner) {
    if (winner == -1) {
        fprintf(details, "DRAW\n");
        players[0].score = 1;
        players[1].score = 1;
    } else {
        players[winner]._log("%s", "WIN\n");
        players[winner].score = 2;
        players[winner^1].score = 0; 
    }
    for (auto &p : players) fprintf(score, "%c: %d\n", 'A'+p.player_id, p.score);
    for (auto &p : players) p.clean();
    exit(0);
}

char board[3][3];

void add(int turn, int x, int y) {
    Player &player = players[turn];
    player._log("%d %d\n", x, y);
    if (x < 0 || x > 2 || y < 0 || y > 2 || board[x][y] != -1) {
        player._log("%s", "invalid operation\n");
        endgame(turn^1);
    }
    board[x][y] = turn;
    const char ch[3] = {' ', 'o', 'x'};
    
    // for (int i=0;i<3;i++) {
    //     for (int j=0;j<3;j++) putchar(ch[board[i][j]]);
    //     putchar('\n');
    // }

    if ((board[x][0] == turn && board[x][1] == turn && board[x][2] == turn) ||
        (board[0][y] == turn && board[1][y] == turn && board[2][y] == turn) ||
        (board[0][0] == turn && board[1][1] == turn && board[2][2] == turn) ||
        (board[0][2] == turn && board[1][1] == turn && board[2][0] == turn)) {
            endgame(turn);
        }
    
    int counter = 0;
    for (int i=0;i<3;i++) for (int j=0;j<3;j++) if (board[i][j] == -1) counter++;
    if (!counter) endgame(-1);
};

void alarm_handler(int) {
    // puts("alarm!");
    alarm(1);
}

int main(int argc, char *argv[]) {
    details = fopen("details.txt", "w");
    score = fopen("score.txt", "w");
    // details = (FILE*) stdout;
    // score = (FILE*) stdout;

    signal(SIGALRM, alarm_handler);
    alarm(1);

    players.push_back(Player("g++ A.cpp -o A", "./A", "fifo_CA", "fifo_AC"));
    players.push_back(Player("g++ B.cpp -o B", "./B", "fifo_CB", "fifo_BC"));
    Player &Alice = players[0];
    Player &Bob = players[1];

    Alice.init();
    Bob.init();

    int x, y;
    memset(board, -1, sizeof(board));
    const int T1 = 1000, T2 = 100;

    Alice.run();
    Alice.setTimeout(T1+T2, false);
    Alice._printf("%d\n", 0);
    Alice._scanf("%d%d", &x, &y);
    Alice.signal_stop();
    Alice.checkTimeout();
    add(0, x, y);

    Bob.run();
    Bob.setTimeout(T1+T2, false);
    Bob._printf("%d\n%d %d\n", 1, x, y);
    Bob._scanf("%d%d", &x, &y);
    Bob.signal_stop();
    Bob.checkTimeout();
    add(1, x, y);

    int turn = 0;
    while (1) {
        Player &player = (turn==0 ? Alice : Bob);
        player.setTimeout(T2, true);
        player.signal_continue();
        player._printf("%d %d\n", x, y);
        player._scanf("%d%d", &x, &y);
        player.signal_stop();
        player.checkTimeout();
        add(turn, x, y);
        turn ^= 1;
    }
}