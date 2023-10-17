#include <cstring>
#include "interactive.h"

char board[3][3];

void add(int now, int x, int y) {
    Player &player = players[now];
    player.log("%d %d\n", x, y);
    if (x < 0 || x > 2 || y < 0 || y > 2 || board[x][y] != -1) {
        players[now].score = 0;
        players[now^1].score = 2;
        players[now].log("%s", "Invalid operation\n");
        Itlib::endgame("Invalid operation");
    }
    board[x][y] = now;
    
    const char ch[3] = {' ', 'o', 'x'};
    string visualize_board = "---\n";
    for (int i=0;i<3;i++) {
        visualize_board += "  |";
        for (int j=0;j<3;j++) visualize_board += ch[board[i][j]+1];
        visualize_board += "|\n";
    }
    visualize_board += "   ---\n";
    player.log("%s", visualize_board.c_str());

    if ((board[x][0] == now && board[x][1] == now && board[x][2] == now) ||
        (board[0][y] == now && board[1][y] == now && board[2][y] == now) ||
        (board[0][0] == now && board[1][1] == now && board[2][2] == now) ||
        (board[0][2] == now && board[1][1] == now && board[2][0] == now)) {
            players[now].score = 2;
            players[now^1].score = 0;
            players[now].log("%s", "Win\n");
            Itlib::endgame("Player win");
        }
    
    int counter = 0;
    for (int i=0;i<3;i++) for (int j=0;j<3;j++) if (board[i][j] == -1) counter++;
    if (!counter) {
        players[0].score = players[1].score = 1;
        Itlib::log("%s", "Draw\n");
        Itlib::endgame("Draw");
    }
};

int main(int argc, char *argv[]) {
    Itlib::init(2, argc, argv, "16m");

    Itlib::playercrashed_handler = [&](int playerID) {
        players[playerID].score = 0;
        players[playerID^1].score = 2;
    };

    Player &Alice = players[0];
    Player &Bob = players[1];

    int x, y;
    memset(board, -1, sizeof(board));
    const int T1 = 5000, T2 = 1000;

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