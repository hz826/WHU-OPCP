#include <cstring>
#include "interactive.h"

char board[15][15];

void add(int now, int x, int y) {
    Player &player = players[now];
    Itlib::log(player, "%d %d\n", x, y);
    if (x < 0 || x > 14 || y < 0 || y > 14 || board[x][y] != -1) {
        players[now].score = 0;
        players[now^1].score = 2;
        Itlib::log(players[now], "%s", "Invalid operation\n");
        Itlib::endgame("Invalid operation");
    }
    board[x][y] = now;
    
    const char ch[3] = {' ', 'o', 'x'};
    string visualize_board = "";
    for (int i=0;i<15;i++) visualize_board += "-";
    visualize_board += "\n";
    for (int i=0;i<15;i++) {
        visualize_board += "  |";
        for (int j=0;j<15;j++) visualize_board += ch[board[i][j]+1];
        visualize_board += "|\n";
    }
    visualize_board += "   ";
    for (int i=0;i<15;i++) visualize_board += "-";
    visualize_board += "\n";
    
    Itlib::log(players[now], "%s", visualize_board.c_str());

    auto win = [&]() {
        players[now].score = 2;
        players[now^1].score = 0;
        Itlib::log(players[now], "%s", "Win\n");
        Itlib::endgame("Someone WIN");
    };

    for (int i=0;i<15;i++) for (int j=0;j<15-4;j++) {
        bool flag = true;
        for (int k=0;k<5;k++) if (board[i][j+k] != now) flag = false;
        if (flag) win();
    }
    for (int i=0;i<15-4;i++) for (int j=0;j<15;j++) {
        bool flag = true;
        for (int k=0;k<5;k++) if (board[i+k][j] != now) flag = false;
        if (flag) win();
    }
    for (int i=0;i<15-4;i++) for (int j=0;j<15-4;j++) {
        bool flag = true;
        for (int k=0;k<5;k++) if (board[i+k][j+k] != now) flag = false;
        if (flag) win();
    }
    for (int i=4;i<15;i++) for (int j=0;j<15-4;j++) {
        bool flag = true;
        for (int k=0;k<5;k++) if (board[i-k][j+k] != now) flag = false;
        if (flag) win();
    }

    int counter = 0;
    for (int i=0;i<15;i++) for (int j=0;j<15;j++) if (board[i][j] == -1) counter++;
    if (!counter) {
        players[0].score = players[1].score = 1;
        Itlib::log("%s", "Draw\n");
        Itlib::endgame("Draw");
    }
};

int main(int argc, char *argv[]) {
    Itlib::init(2, argc, argv, "256m");

    Itlib::playercrashed_handler = [&](int playerID) {
        players[playerID].score = 0;
        players[playerID^1].score = 2;
    };

    Player &Alice = players[0];
    Player &Bob = players[1];

    int x, y;
    memset(board, -1, sizeof(board));
    const int T1 = 10000, T2 = 5000;

    Alice.startRound(T1+T2);
    Alice.printf("%d\n", 0);
    Alice.scanf("%d%d", &x, &y);
    Alice.finishRound();
    add(0, x, y);

    Bob.startRound(T1+T2);
    Bob.printf("%d\n%d %d\n", 1, x, y);
    Bob.scanf("%d%d", &x, &y);
    Bob.finishRound();
    add(1, x, y);

    int now = 0;
    while (1) {
        Player &player = (now==0 ? Alice : Bob);
        player.startRound(T2);
        player.printf("%d %d\n", x, y);
        player.scanf("%d%d", &x, &y);
        player.finishRound();
        add(now, x, y);
        now ^= 1;
    }
}