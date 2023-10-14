#include "interactive.h"

void endgame(int playerID, const char* result) {
    vector<int> score(Player::player_cnt, 0);
    if (result == "JE") {
        fprintf(fp_details, "JUDGE ERROR\n");
        score[0] = 0;
        score[1] = 0;
    } else if (result == "RE") {
        players[playerID]._log("RUNTIME ERROR\n");
        score[playerID] = 0;
        score[playerID^1] = 2;
    } else if (result == "TLE") {
        players[playerID]._log("TIME LIMIT EXCEEDED\n");
        score[playerID] = 0;
        score[playerID^1] = 2;
    } else if (result == "IO") {
        players[playerID]._log("INVALID OPERATION\n");
        score[playerID] = 0;
        score[playerID^1] = 2;
    } else if (result == "DRAW") {
        fprintf(fp_details, "DRAW\n");
        score[0] = 1;
        score[1] = 1;
    } else {
        players[playerID]._log("WIN\n");
        score[playerID] = 2;
        score[playerID^1] = 0;
    }
    for (int i=0;i<Player::player_cnt;i++) fprintf(fp_score, "%c: %d\n", char('A'+i), score);
    // system("read -p \"Press [Enter] key to continue...\" REPLY"); // pause

    for (auto &p : players) p.clean();
    exit(0);
}

char board[3][3];

void add(int turn, int x, int y) {
    Player &player = players[turn];
    player._log("%d %d\n", x, y);
    if (x < 0 || x > 2 || y < 0 || y > 2 || board[x][y] != -1) {
        endgame(turn, "IO");
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
            endgame(turn, "WIN");
        }
    
    int counter = 0;
    for (int i=0;i<3;i++) for (int j=0;j<3;j++) if (board[i][j] == -1) counter++;
    if (!counter) endgame(-1, "DRAW");
};

int main(int argc, char *argv[]) {
    signal(SIGALRM, alarm_handler);
    alarm(1);
    fp_details = fopen("details.txt", "w");
    fp_score = fopen("score.txt", "w");

    players.push_back(Player("g++ main.cpp -o main", "./main", "16m"));
    players.push_back(Player("g++ main.cpp -o main", "./main", "16m"));

    Player &Alice = players[0];
    Player &Bob = players[1];

    Alice.init();
    Bob.init();

    int x, y;
    memset(board, -1, sizeof(board));
    const int T1 = 5000, T2 = 5000;

    Alice.startRound(T1+T2);
    Alice._printf("%d\n", 0);
    Alice._scanf("%d%d", &x, &y);
    Alice.finishRound();
    add(0, x, y);

    Bob.startRound(T1+T2);
    Bob._printf("%d\n%d %d\n", 1, x, y);
    Bob._scanf("%d%d", &x, &y);
    Bob.finishRound();
    add(1, x, y);

    int turn = 0;
    while (1) {
        Player &player = (turn==0 ? Alice : Bob);
        player.startRound(T2);
        player._printf("%d %d\n", x, y);
        player._scanf("%d%d", &x, &y);
        player.finishRound();
        add(turn, x, y);
        turn ^= 1;
    }
}