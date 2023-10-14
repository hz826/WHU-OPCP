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

FILE *fp_details, *fp_score;

class Player;
vector<Player> players;
int player_now = -1;

void alarm_handler(int);
void endgame(int, const char*);
void halt();

class Player {
public:
    static int player_cnt;

private:
    int playerID, score, round_cnt;
    bool folder_created, container_created, system_check_return_value;

    unsigned long long lastTime_used, lastTime_real;
    int timeout;

    string compile_cmd, run_cmd, memory_limit, player_name, player_folder;
    string fifo_in, fifo_out, tmp_file, container_name, container_longid;

    int fd_in, fd_out, exec_pid;
    FILE *fp_in, *fp_out;

    std::pair<unsigned long long, unsigned long long> getTime() {
        // cgroup v1
        unsigned long long user_time_ns, sys_time_ns;
        string cgroup_cpuacct_path = "/sys/fs/cgroup/cpuacct/docker/" + container_longid + "/";
        FILE *fp;
        fp = fopen((cgroup_cpuacct_path+"cpuacct.usage_user").c_str(), "r");
        if (fp == NULL) _halt("cannot read cgroup cpustat status");
        fscanf(fp, "%llu", &user_time_ns);
        fclose(fp);
        fp = fopen((cgroup_cpuacct_path+"cpuacct.usage_sys").c_str(), "r");
        if (fp == NULL) _halt("cannot read cgroup cpustat status");
        fscanf(fp, "%llu", &sys_time_ns);
        fclose(fp);
        unsigned long long total_time_used_ms = (user_time_ns + sys_time_ns) / 1000000;

        struct timeval tv;
        gettimeofday(&tv, NULL);
        unsigned long long total_time_real_ms = (unsigned long long) tv.tv_sec * 1000 + tv.tv_usec / 1000;

        return std::make_pair(total_time_used_ms, total_time_real_ms);
    }

    std::pair<int, int> getTimeUsage() {
        auto p = getTime();
        return std::make_pair(p.first-lastTime_used, p.second-lastTime_real);
    }

    void setTimeout(int timeout_ms) {
        auto p = getTime();
        lastTime_used = p.first;
        lastTime_real = p.second;
        timeout = timeout_ms;
    }

    double getMemoryUsage() {
        // cgroup v1
        unsigned long long usage, limit;
        string cgroup_memory_path = "/sys/fs/cgroup/memory/docker/" + container_longid + "/";
        FILE *fp;

        fp = fopen((cgroup_memory_path+"memory.max_usage_in_bytes").c_str(), "r");
        if (fp == NULL) _halt("cannot read cgroup memory status");
        fscanf(fp, "%llu", &usage);
        fclose(fp);

        fp = fopen((cgroup_memory_path+"memory.limit_in_bytes").c_str(), "r");
        if (fp == NULL) _halt("cannot read cgroup memory status");
        fscanf(fp, "%llu", &limit);
        fclose(fp);

        double precent = 100.0 * usage / limit;
        _log("MemoryUsage : %llu / %llu = %.1f%%\n", usage, limit, precent);
    }

    template<typename... Args>
    void _system(Args... args) {
        char cmd[1024];
        sprintf(cmd, args...);
        _log(">>> %s\n", cmd);
        if (::system(cmd) && system_check_return_value) _halt("failed on executing bash command");
    }

    void signal_stop() {
        _system("docker pause %s > /dev/null", container_name.c_str());
    }

    void signal_continue() {
        _system("docker unpause %s > /dev/null", container_name.c_str());
    }

    void _halt(const char* errmsg) {
        _log("HALT | %s\n", errmsg);
        fflush(fp_details);
        halt();
    }

public:
    Player(string compile_cmd, string run_cmd, string memory_limit) 
        : compile_cmd(compile_cmd), run_cmd(run_cmd), memory_limit(memory_limit) {
            playerID = player_cnt++;
            player_name = char('A'+playerID);
            container_name = "sandbox_" + player_name;
            player_folder = "tmp_" + player_name + "/";
            fifo_in = player_folder + "fifo_in";
            fifo_out = player_folder + "fifo_out";
            tmp_file = player_folder + "tmp";
            folder_created = container_created = false;
            system_check_return_value = true;
        }

    void init() {
        if (access(player_folder.c_str(), F_OK)) _system("rm -rf %s", player_folder.c_str());
        if (mkdir(player_folder.c_str(),0777) < 0) _halt("cannot create folder");
        folder_created = true;

        if (mkfifo(fifo_in.c_str(),0777) < 0) _halt("cannot create fifo");
        if (mkfifo(fifo_out.c_str(),0777) < 0) _halt("cannot create fifo");

        _system("docker run --name %s -d gcc sh -c \"while true; do sleep 1; done\" > %s", 
                container_name.c_str(), tmp_file.c_str());
        container_created = true;

        char cid[70];
        FILE *fp = fopen(tmp_file.c_str(), "r");
        if (fp != NULL) {
            fscanf(fp, "%s", cid);
            container_longid = cid;
            fclose(fp);
        }
        if (container_longid.length() != 64) _halt("cannot get container_longid");

        _system("docker cp %s %s:/code", player_name.c_str(), container_name.c_str());
        _system("docker exec -w /code %s %s", container_name.c_str(), compile_cmd.c_str());
        _system("docker restart %s > /dev/null", container_name.c_str());
    }

    void run() {
        _system("docker update --memory %s --memory-swap %s %s > /dev/null", 
                memory_limit.c_str(), memory_limit.c_str(), container_name.c_str());
        
        exec_pid = fork();
        if (exec_pid < 0) _halt("cannot fork");
        if (exec_pid == 0) {
            system_check_return_value = false;
            _system("docker exec -i -w /code %s %s < %s > %s", 
                    container_name.c_str(), run_cmd.c_str(), fifo_in.c_str(), fifo_out.c_str());
            exit(0);
        }

        if ((fd_in  = open(fifo_in.c_str(),  O_WRONLY)) < 0) _halt("cannot open fifo\n");
        if ((fd_out = open(fifo_out.c_str(), O_RDONLY)) < 0) _halt("cannot open fifo\n");

        if ((fp_in  = fdopen(fd_in,  "w")) == NULL) _halt("cannot fdopen fifo\n");
        if ((fp_out = fdopen(fd_out, "r")) == NULL) _halt("cannot fdopen fifo\n");
    }

    void startRound(int Timeout_ms) {
        setTimeout(Timeout_ms);
        if (++round_cnt == 1) {
            run();
        } else {
            signal_continue();
        }
        player_now = playerID;
    }

    void checkTimeout() { 
        int wstatus;
        int result = waitpid(exec_pid, &wstatus, WNOHANG);
        if (result < 0) _halt("waitpid error");
        if (result == exec_pid) {
            _log("Exited abnormally\n");
            double m = getMemoryUsage();
            if (m > 98) endgame(playerID, "MLE");
            endgame(playerID, "RE");
        }

        auto p = getTimeUsage();
        int used = p.first;
        int real = p.second;
        _log("TimeUsage : used=%d ms  real=%d ms\n", used, real);
        if (used > timeout) endgame(playerID, "TLE");
        if (real > 3*timeout+1000) endgame(playerID, "TLE");
    }

    void finishRound() {
        player_now = -1;
        signal_stop();
        checkTimeout();
        getMemoryUsage();
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
        fprintf(fp_details, "%s: ", player_name.c_str());
        fprintf(fp_details, args...);
        fflush(fp_details);
    }

    void clean() {
        system_check_return_value = false;
        if (container_created) _system("docker rm -f %s > /dev/null", container_name.c_str());
        if (folder_created) _system("rm -rf %s", player_folder.c_str());
    }
};
int Player::player_cnt = 0;

void halt() {
    for (auto &p : players) p.clean();
    exit(1);
}

void alarm_handler(int) {
    if (player_now < 0) return ;
    players[player_now].checkTimeout();
    alarm(1);
}