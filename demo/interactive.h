#pragma once
#include <string>
#include <vector>
#include <functional>
using std::string;
using std::vector;
using std::pair;
using std::function;

class Player {
public:
    static int player_cnt, player_now;
    int score;

    Player(string image_name, string code_folder, string compile_cmd, string run_cmd, string memory_limit);
    void startRound(int Timeout_ms);
    void checkTimeout();
    void finishRound();
    void clean();

    template<typename... Args>
    void scanf(Args... args);
    template<typename... Args>
    void printf(Args... args);
    string getname() const;

private:
    int playerID, round_cnt;
    bool tmp_folder_created, container_created, system_check_return_value;

    unsigned long long lastTime_used, lastTime_real;
    int timeout;

    string image_name, code_folder, compile_cmd, run_cmd, memory_limit, player_name, tmp_folder;
    string fifo_in, fifo_out, tmp_file, container_name, container_longid;

    int fd_in, fd_out, exec_pid;
    FILE *fp_in, *fp_out;

    void create();
    void compile();
    void run();
    pair<unsigned long long, unsigned long long> getTime();
    pair<int, int> getTimeUsage();
    void setTimeout(int timeout_ms);
    double getMemoryUsage();

    void signal_stop();
    void signal_continue();
    void halt(const char* errmsg);

    template<typename... Args>
    void system(Args... args);
    template<typename... Args>
    void log(Args... args);
};

extern vector<Player> players;

class Itlib {
    public:
    static string prefix;
    static FILE *fp_details, *fp_score;
    static function<void(int)> playercrashed_handler;
    static void init(int player_cnt, int argc, char *argv[], string memory_limit);

    static void endgame(string msg);
    static void internalError();
    
    template<typename... Args>
    static void log(const Player &player, const char* format, Args... args);

    template<typename... Args>
    static void log(const char* format, Args... args);
};

template<typename... Args>
void Player::scanf(Args... args) {
    fscanf(fp_out, args...);
}

template<typename... Args>
void Player::printf(Args... args) {
    fprintf(fp_in, args...);
    fflush(fp_in);
}

template<typename... Args>
void Player::log(Args... args) {
    fprintf(Itlib::fp_details, "%s: ", player_name.c_str());
    fprintf(Itlib::fp_details, args...);
    fflush(Itlib::fp_details);
}

template<typename... Args>
void Player::system(Args... args) {
    char cmd[1024];
    sprintf(cmd, args...);
    log("[Debug] >>> %s\n", cmd);
    if (::system(cmd) && system_check_return_value) halt("failed on executing bash command");
}

template<typename... Args>
void Itlib::log(const Player &player, const char* format, Args... args) {
    fprintf(Itlib::fp_details, "%s: ", player.getname().c_str());
    fprintf(Itlib::fp_details, format, args...);
    fflush(Itlib::fp_details);
}

template<typename... Args>
void Itlib::log(const char* format, Args... args) {
    fprintf(Itlib::fp_details, "=: ");
    fprintf(Itlib::fp_details, format, args...);
    fflush(Itlib::fp_details);
}