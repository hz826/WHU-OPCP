#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
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
void endgame(int);

class Player {
    public:
    int player_id, score, last_timeUsage;
    string compile_cmd, run_cmd, memory_limit, fifo_in, fifo_out, player_name, tmp_file, container_name, container_longid;
    int fd_in, fd_out, pid;
    FILE *fp_in, *fp_out; 
    static int player_cnt;

    Player(string compile_cmd, string run_cmd, string memory_limit) 
        : compile_cmd(compile_cmd), run_cmd(run_cmd), memory_limit(memory_limit) {
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

        char cid[70];
        FILE *fp = fopen(tmp_file.c_str(), "r");
        fscanf(fp, "%s", cid);
        container_longid = cid;
        fclose(fp);

        _system("docker cp %s %s:/code", player_name.c_str(), container_name.c_str());
        _system("docker exec -w /code %s %s", container_name.c_str(), compile_cmd.c_str());

        // cgroup v1
        // _system("echo %s > /sys/fs/cgroup/memory/docker/%s/memory.limit_in_bytes", memory_limit.c_str(), container_longid.c_str());
    }

    void clean() {
        _system("docker rm -f %s > /dev/null", container_name.c_str());
        remove(tmp_file.c_str());
        unlink(fifo_in.c_str());
        unlink(fifo_out.c_str());
    }

    void run() {
        _system("docker update --memory %s --memory-swap %s %s > /dev/null", memory_limit.c_str(), memory_limit.c_str(), container_name.c_str());
        
        pid = fork();
        if (pid < 0) { printf("cannot fork\n");  exit(1); }
        if (pid == 0) {
            _system("docker exec -i -w /code %s %s < %s > %s", container_name.c_str(), run_cmd.c_str(), fifo_in.c_str(), fifo_out.c_str());
            exit(0);
        }

        if ((fd_in = open(fifo_in.c_str(), O_WRONLY)) < 0) { printf("cannot open fifo\n");  exit(1); }
        if ((fd_out = open(fifo_out.c_str(), O_RDONLY)) < 0) { printf("cannot open fifo\n");  exit(1); }

        if ((fp_in = fdopen(fd_in, "w")) == NULL) { printf("cannot fdopen fifo\n");  exit(1); }
        if ((fp_out = fdopen(fd_out, "r")) == NULL) { printf("cannot fdopen fifo\n");  exit(1); }
    }

    int getTimeUsage() {
        // cgroup v1
        unsigned long long user_time_ns, sys_time_ns;
        string cgroup_cpuacct_path = "/sys/fs/cgroup/cpuacct/docker/" + container_longid + "/";
        FILE *fp;

        fp = fopen((cgroup_cpuacct_path+"cpuacct.usage_user").c_str(), "r");
        fscanf(fp, "%llu", &user_time_ns);
        fclose(fp);

        fp = fopen((cgroup_cpuacct_path+"cpuacct.usage_sys").c_str(), "r");
        fscanf(fp, "%llu", &sys_time_ns);
        fclose(fp);

        int total_time_ms = (user_time_ns + sys_time_ns) / 1000000;
        return total_time_ms;
    }

    double getMemoryUsage() {
        // cgroup v1
        unsigned long long usage, limit;
        string cgroup_memory_path = "/sys/fs/cgroup/memory/docker/" + container_longid + "/";
        FILE *fp;

        fp = fopen((cgroup_memory_path+"memory.max_usage_in_bytes").c_str(), "r");
        fscanf(fp, "%llu", &usage);
        fclose(fp);

        fp = fopen((cgroup_memory_path+"memory.limit_in_bytes").c_str(), "r");
        fscanf(fp, "%llu", &limit);
        fclose(fp);

        double precent = 100.0 * usage / limit;
        _log("MemoryUsage : %llu / %llu = %.1f%%\n", usage, limit, precent);
    }

    void setTimeout(int T) {
        last_timeUsage = getTimeUsage();

        // player_now = player_id;
        // struct itimerval tick;
        // memset(&tick, 0, sizeof(tick));
        // tick.it_value.tv_sec = T / 1000;
        // tick.it_value.tv_usec = T % 1000;
        // if (setitimer(ITIMER_REAL, &tick, NULL)) { printf("set timer failed!!/n");  exit(1); }
    }

    void checkTimeout() {
        int wstatus;
        int result = waitpid(pid, &wstatus, WNOHANG);
        if (result == pid) {
            _log("%s\n", "Exited abnormally");
            getMemoryUsage();
            endgame(player_id^1);
        }

        int total_time_ms = getTimeUsage();
        _log("TimeUsage : %d ms\n", total_time_ms - last_timeUsage);
        getMemoryUsage();

        // struct itimerval tick;
        // memset(&tick, 0, sizeof(tick));
        // if (setitimer(ITIMER_REAL, &tick, NULL)) { printf("reset timer failed!!/n");  exit(1); }
        // player_now = -1;
    }

    void signal_stop() {
        _system("docker pause %s > /dev/null", container_name.c_str());
    }

    void signal_continue() {
        _system("docker unpause %s > /dev/null", container_name.c_str());
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
    // if (player_now < 0) { printf("alarm error/n");  exit(1); }
    // players[player_now]._log("%s", "time limit exceeded\n");
    // endgame(player_now^1);
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

    players.push_back(Player("g++ main.cpp -o main", "./main", "128m"));
    players.push_back(Player("g++ main.cpp -o main", "./main", "128m"));

    Player &Alice = players[0];
    Player &Bob = players[1];

    Alice.init();
    Bob.init();

    int x, y;
    memset(board, -1, sizeof(board));
    const int T1 = 5000, T2 = 5000;

    Alice.setTimeout(T1+T2);
    Alice.run();
    Alice._printf("%d\n", 0);
    Alice._scanf("%d%d", &x, &y);
    Alice.signal_stop();
    Alice.checkTimeout();
    add(0, x, y);

    Bob.setTimeout(T1+T2);
    Bob.run();
    Bob._printf("%d\n%d %d\n", 1, x, y);
    Bob._scanf("%d%d", &x, &y);
    Bob.signal_stop();
    Bob.checkTimeout();
    add(1, x, y);

    int turn = 0;
    while (1) {
        Player &player = (turn==0 ? Alice : Bob);
        player.setTimeout(T2);
        player.signal_continue();
        player._printf("%d %d\n", x, y);
        player._scanf("%d%d", &x, &y);
        player.signal_stop();
        player.checkTimeout();
        add(turn, x, y);
        turn ^= 1;
    }
}