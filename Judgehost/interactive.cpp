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
#include "interactive.h"
using std::string;
using std::vector;
using std::pair;
using std::make_pair;

FILE *fp_details, *fp_score;

vector<Player> players;

int Player::player_cnt = 0;
int Player::player_now = -1;

string Player::getname() const {
    return player_name;
}

void Player::create() {
    system("mkdir %s", tmp_folder.c_str());
    tmp_folder_created = true;

    if (mkfifo(fifo_in.c_str(),0777) < 0) halt("cannot create fifo");
    if (mkfifo(fifo_out.c_str(),0777) < 0) halt("cannot create fifo");

    system("docker run --name %s -d %s sh -c \"while true; do sleep 1; done\" > %s", 
            container_name.c_str(), image_name.c_str(), tmp_file.c_str());
    container_created = true;

    char cid[70];
    FILE *fp = fopen(tmp_file.c_str(), "r");
    if (fp != NULL) {
        fscanf(fp, "%s", cid);
        container_longid = cid;
        fclose(fp);
    }
    if (container_longid.length() != 64) halt("cannot get container_longid");

    system("docker cp %s %s:/code", code_folder.c_str(), container_name.c_str());
}

void Player::compile() {
    system("docker exec -w /code %s %s >> %s 2>&1", container_name.c_str(), compile_cmd.c_str(), tmp_file.c_str());
    system("docker restart %s > /dev/null", container_name.c_str());
}

void Player::run() {
    system("docker update --memory %s --memory-swap %s %s > /dev/null", 
            memory_limit.c_str(), memory_limit.c_str(), container_name.c_str());
    
    exec_pid = fork();
    if (exec_pid < 0) halt("cannot fork");
    if (exec_pid == 0) {
        system_check_return_value = false;
        system("docker exec -i -w /code %s %s < %s > %s 2>> %s", 
                container_name.c_str(), run_cmd.c_str(), fifo_in.c_str(), fifo_out.c_str(), tmp_file.c_str());
        exit(0);
    }

    if ((fd_in  = open(fifo_in.c_str(),  O_WRONLY)) < 0) halt("cannot open fifo\n");
    if ((fd_out = open(fifo_out.c_str(), O_RDONLY)) < 0) halt("cannot open fifo\n");

    if ((fp_in  = fdopen(fd_in,  "w")) == NULL) halt("cannot fdopen fifo\n");
    if ((fp_out = fdopen(fd_out, "r")) == NULL) halt("cannot fdopen fifo\n");
}

pair<unsigned long long, unsigned long long> Player::getTime() {
    // cgroup v1
    unsigned long long user_time_ns, sys_time_ns;
    string cgroup_cpuacct_path = "/sys/fs/cgroup/cpuacct/docker/" + container_longid + "/";
    FILE *fp;
    fp = fopen((cgroup_cpuacct_path+"cpuacct.usage_user").c_str(), "r");
    if (fp == NULL) halt("cannot read cgroup cpustat status");
    fscanf(fp, "%llu", &user_time_ns);
    fclose(fp);
    fp = fopen((cgroup_cpuacct_path+"cpuacct.usage_sys").c_str(), "r");
    if (fp == NULL) halt("cannot read cgroup cpustat status");
    fscanf(fp, "%llu", &sys_time_ns);
    fclose(fp);
    unsigned long long total_time_used_ms = (user_time_ns + sys_time_ns) / 1000000;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long long total_time_real_ms = (unsigned long long) tv.tv_sec * 1000 + tv.tv_usec / 1000;

    return make_pair(total_time_used_ms, total_time_real_ms);
}

pair<int, int> Player::getTimeUsage() {
    auto p = getTime();
    return make_pair(p.first-lastTime_used, p.second-lastTime_real);
}

void Player::setTimeout(int timeout_ms) {
    auto p = getTime();
    lastTime_used = p.first;
    lastTime_real = p.second;
    timeout = timeout_ms;
}

double Player::getMemoryUsage() {
    // cgroup v1
    unsigned long long usage, limit;
    string cgroup_memory_path = "/sys/fs/cgroup/memory/docker/" + container_longid + "/";
    FILE *fp;

    fp = fopen((cgroup_memory_path+"memory.max_usage_in_bytes").c_str(), "r");
    if (fp == NULL) halt("cannot read cgroup memory status");
    fscanf(fp, "%llu", &usage);
    fclose(fp);

    fp = fopen((cgroup_memory_path+"memory.limit_in_bytes").c_str(), "r");
    if (fp == NULL) halt("cannot read cgroup memory status");
    fscanf(fp, "%llu", &limit);
    fclose(fp);

    double precent = 100.0 * usage / limit;
    log("[Limit] MemoryUsage : %llu / %llu = %.1f%%\n", usage, limit, precent);
    return precent;
}

void Player::signal_stop() {
    system("docker pause %s > /dev/null", container_name.c_str());
}

void Player::signal_continue() {
    system("docker unpause %s > /dev/null", container_name.c_str());
}

void Player::halt(const char* errmsg) {
    log("HALT | %s\n", errmsg);
    fflush(fp_details);
    Itlib::internalError();
}

Player::Player(string image_name, string code_folder, string compile_cmd, string run_cmd, string memory_limit) 
    : image_name(image_name), code_folder(code_folder), compile_cmd(compile_cmd), run_cmd(run_cmd), memory_limit(memory_limit) {
    playerID = player_cnt++;
    player_name = char('A'+playerID);
    container_name = "sandbox_" + Itlib::prefix + "_" + player_name;
    // tmp_folder = Itlib::prefix + "/tmp_" + player_name + "/";
    tmp_folder = "tmp_" + player_name + "/";
    fifo_in = tmp_folder + "fifo_in";
    fifo_out = tmp_folder + "fifo_out";
    tmp_file = tmp_folder + "tmp";
    tmp_folder_created = container_created = false;
    system_check_return_value = true;
    round_cnt = 0;
    score = 0;

    create();
    compile();
}

void Player::startRound(int Timeout_ms) {
    log("[Debug] %s", "startRound\n");

    setTimeout(Timeout_ms);
    if (++round_cnt == 1) {
        run();
    } else {
        signal_continue();
    }
    player_now = playerID;
}

void Player::checkTimeout() { 
    log("[Debug] %s", "checkTimeout start\n");
    int wstatus;
    int result = waitpid(exec_pid, &wstatus, WNOHANG);
    if (result < 0) halt("waitpid error");
    if (result == exec_pid) {
        log("[Debug] %s", "Exited abnormally\n");
        double m = getMemoryUsage();

        Itlib::playercrashed_handler(playerID);
        if (m > 98) Itlib::endgame("MEMORY LIMIT EXECEED");
        else Itlib::endgame("RUNTIME ERROR");
    }

    auto p = getTimeUsage();
    int used = p.first;
    int real = p.second;
    log("[Limit] TimeUsage : used=%d ms  real=%d ms\n", used, real);

    if (used > timeout || real > 3*timeout+1000) {
        Itlib::playercrashed_handler(playerID);
        Itlib::endgame("TIME LIMIT EXECEED");
    }
    log("[Debug] %s", "checkTimeout finished\n");
}

void Player::finishRound() {
    player_now = -1;
    signal_stop();
    checkTimeout();
    getMemoryUsage();
    log("[Debug] %s", "finishRound\n");
}

void Player::clean() {
    system_check_return_value = false;
    if (container_created) system("docker rm -f %s > /dev/null", container_name.c_str());
    if (tmp_folder_created) system("rm -rf %s", tmp_folder.c_str());
}

void alarm_handler(int) {
    if (Player::player_now < 0) return ;
    Itlib::log("[Debug] %s", "ALARM\n");
    players[Player::player_now].checkTimeout();
    alarm(1);
}

FILE* Itlib::fp_details = NULL;
FILE* Itlib::fp_score = NULL;
string Itlib::prefix = "";

function<void(int)> Itlib::playercrashed_handler = [&](int x) {
    Itlib::log("[Debug] %s", "playercrashed_handler haven't set\n");
};

void Itlib::init(int player_cnt, int argc, char *argv[], string memory_limit) {
    fp_details = fopen("details.txt", "w");
    fp_score = fopen("score.txt", "w");

    if (argc != player_cnt*4+2) {
        Itlib::log("[Debug] %s", "incorrect argument count\n");
        Itlib::internalError();
    }
    signal(SIGALRM, alarm_handler);
    alarm(1);

    Itlib::prefix = argv[1];
    int index = 1;

    for (int i=0;i<player_cnt;i++) {
        string folder_path = argv[++index];
        string image_name = argv[++index];
        string compile_cmd = argv[++index];
        string run_cmd = argv[++index];
        players.push_back(Player(image_name, folder_path, compile_cmd, run_cmd, memory_limit));
    }
}

void Itlib::endgame(string msg) {
    for (auto &p : players) p.clean();
    Itlib::log("[Debug] %s\n", msg.c_str());
    fprintf(fp_score, "%s\n", msg.c_str());
    char c = 'A';
    for (auto &p : players) fprintf(fp_score, "%c: %d\n", c++, p.score);
    exit(0);
}

void Itlib::internalError() {
    fprintf(fp_score, "%s\n", "InternalError");
    for (auto &p : players) p.clean();
    exit(1);
}