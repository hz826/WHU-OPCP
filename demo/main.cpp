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
int player_now;

void alarm_handler(int);

class Player {
    public:
    int player_id, score;
    string compile_cmd, run_cmd, fifo_in, fifo_out, player_name, tmp_file, container_name, container_id;
    int fd_in, fd_out, pid;
    FILE *fp_in, *fp_out; 
    static int player_cnt;

    Player(string compile_cmd, string run_cmd) 
        : compile_cmd(compile_cmd), run_cmd(run_cmd) {
            player_id = player_cnt++;
            player_name = char('A'+player_id);
            fifo_in = string("fifo_in_") + player_name;
            fifo_out = string("fifo_out_") + player_name;
            tmp_file = string("tmp_") + player_name;
            container_name = string("sandbox_") + player_name;
        }

    void init() {
        unlink(fifo_in.c_str());
        unlink(fifo_out.c_str());
        if (mkfifo(fifo_in.c_str(),0777) < 0) { printf("cannot create fifo\n");  exit(1); } 
        if (mkfifo(fifo_out.c_str(),0777) < 0) { printf("cannot create fifo\n");  exit(1); }

        _system("docker run --name %s -d gcc sh -c \"while true; do sleep 1; done\" > %s", container_name.c_str(), tmp_file.c_str());

        {
            char cid[70];
            FILE *fp = fopen(tmp_file.c_str(), "r");
            fscanf(fp, "%s", cid);
            container_id = cid;
            fclose(fp);
        }
        _log("container_id : %s\n", container_id.c_str());

        _system("docker cp %s %s:/code", player_name.c_str(), container_name.c_str());
        _system("docker exec %s %s", container_name.c_str(), compile_cmd.c_str());
    }

    void clean() {
        _system("docker rm -f %s > /dev/null", container_name.c_str());
        unlink(fifo_in.c_str());
        unlink(fifo_out.c_str());
        signal_kill();
    }

    void run() {
        _system("docker exec -i %s %s < %s > %s &", container_name.c_str(), run_cmd.c_str(), fifo_in.c_str(), fifo_out.c_str());

        // FILE *fp = fopen(tmp_file.c_str(), "r");
        // fscanf(fp, "%d", &pid);
        // fclose(fp);
        // _log("pid = %d\n", pid);

        if ((fd_in = open(fifo_in.c_str(), O_WRONLY)) < 0) { printf("cannot open fifo\n");  exit(1); }
        if ((fd_out = open(fifo_out.c_str(), O_RDONLY)) < 0) { printf("cannot open fifo\n");  exit(1); }

        if ((fp_in = fdopen(fd_in, "w")) == NULL) { printf("cannot fdopen fifo\n");  exit(1); }
        if ((fp_out = fdopen(fd_out, "r")) == NULL) { printf("cannot fdopen fifo\n");  exit(1); }
    }

    int getUserTime() {
        // string stat_path = "/proc/" + std::to_string(pid) + "/stat";
        // FILE *fp = fopen(stat_path.c_str(), "r");
        // fscanf(fp, "%*d%*s%*s%*d%*d");
        // fscanf(fp, "%*d%*d%*d%*d%*d");
        // fscanf(fp, "%*d%*d%*d");

        // unsigned long utime;
        // fscanf(fp, "%lu", &utime);
        // _log("usertime : %lu\n", utime);
        // fclose(fp);
    }

    void setTimeout(int T, bool resetTimer) {
        player_now = player_id;
        struct itimerval tick;
        memset(&tick, 0, sizeof(tick));
        tick.it_value.tv_sec = T / 1000;
        tick.it_value.tv_usec = T % 1000;
        if (setitimer(ITIMER_REAL, &tick, NULL)) { printf("set timer failed!!/n");  exit(1); }
    }

    void checkTimeout() {
        getUserTime();
        struct itimerval tick;
        memset(&tick, 0, sizeof(tick));
        if (setitimer(ITIMER_REAL, &tick, NULL)) { printf("reset timer failed!!/n");  exit(1); }
        player_now = -1;
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
    void _system(Args... args) {
        char cmd[1024];
        sprintf(cmd, args...);
        _log(">>> %s\n", cmd);
        ::system(cmd);
    }

    template<typename... Args>
    void _log(Args... args) {
        fprintf(details, "%c: ", 'A'+player_id);
        fprintf(details, args...);
        fflush(details);
    }
};
int Player::player_cnt = 0;

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

    system("read -p \"Press [Enter] key to continue...\" REPLY"); // pause

    for (auto &p : players) p.clean();
    exit(0);
}

void alarm_handler(int) {
    if (player_now < 0) { printf("alarm error/n");  exit(1); }
    players[player_now]._log("%s", "time limit exceeded\n");
    endgame(player_now^1);
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
    
    string visualize_board = "---\n";
    for (int i=0;i<3;i++) {
        visualize_board += "  |";
        for (int j=0;j<3;j++) visualize_board += ch[board[i][j]+1];
        visualize_board += "|\n";
    }
    visualize_board += "   ---\n";
    player._log("%s", visualize_board.c_str());

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

int main(int argc, char *argv[]) {
    signal(SIGALRM, alarm_handler);

    details = fopen("details.txt", "w");
    score = fopen("score.txt", "w");
    // details = (FILE*) stdout;
    // score = (FILE*) stdout;  // for debug

    players.push_back(Player("g++ code/main.cpp -o code/main", "code/main"));
    players.push_back(Player("g++ code/main.cpp -o code/main", "code/main"));

    Player &Alice = players[0];
    Player &Bob = players[1];

    Alice.init();
    Bob.init();

    int x, y;
    memset(board, -1, sizeof(board));
    const int T1 = 5000, T2 = 5000;

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