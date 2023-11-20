#include <iostream>
// #include <conio.h>
#include <ctime>
#include <cstdlib>
using namespace std;
 
struct point{
    int y;
    int x;
}; 
struct dir{
    int dy;
    int dx;
};
const dir d1 = {0, 1};//横 
const dir d2 = {1, 0};//竖 
const dir d3 = {1, -1};//撇 
const dir d4 = {1, 1};//捺 

class gobang{
private:
    int chessboard[15][15];//记录棋盘信息 
    int player;//玩家棋子 
    int computer;//电脑棋子 
    int Y, X;//光标位置 
    point laozi;//落子位置 
public:
    void play();
        int selMode();
        void turnPlayer();
        void turnComputer(); 
        bool isEnd();
protected:
    bool isInBoard(point p); 
    void display();
    int score(point p, int who);
    point newPoint(point p, dir d, int lenth);
};
void gobang::play(){
    int i = selMode();
    if(i == 1){
        computer = 1;
        player = 2;
    }
    else{
        player = 1;
        computer = 2;
    }
    int cur = 1;
    while(1){
        if(cur == player){
            turnPlayer();
        }
        else{
            turnComputer();
        }
        if(isEnd()){
            // if(cur == player){
            //     cout << "Player Win !";
            //     system("pause");
            //     break;
            // }    
            // else{
            //     cout << "Computer Win !";
            //     system("pause");
            //        break; 
            // }
            return ;
        }
        else{
            cur = (cur == 1) ? 2 : 1;
        }        
    }
} 
int gobang::selMode(){
    // system("cls");
    // cout << "*************************************************" << endl;
    // cout << "******************0、退出************************" << endl;
    // cout << "******************1、电脑先手********************" << endl;
    // cout << "******************2、玩家先手********************" << endl;
    // cout << "*************************************************" << endl;
    // while(1){
    //     int i;
    //     cin >> i;
    //     if(i == 0){
    //         exit(1);
    //     }
    //     else if(i == 1 || i == 2){
    //         Y = 9;
    //         X = 9;
    //         for(int j = 0; j < 15; j ++){
    //             for(int k = 0; k < 15; k ++){
    //                 chessboard[j][k] = 0;
    //             }
    //         }
    //         display();
    //         return i;
    //     }
    //     else{
    //         cout << "非法输入，请重新输入！" << endl;
    //     }
    // }
    int i;
    scanf("%d", &i);
    ++i;

    Y = 9;
    X = 9;
    for(int j = 0; j < 15; j ++){
        for(int k = 0; k < 15; k ++){
            chessboard[j][k] = 0;
        }
    }
    return i;
}
void gobang::turnPlayer(){
    int X, Y;
    cin >> Y >> X;
    laozi.y = Y;
    laozi.x = X;
    chessboard[Y][X] = player;
    // display();
}
void gobang::turnComputer(){
    //cout << "turn computer" << endl; 
    point best1, best2;
    do{
        srand(time(NULL));
        best1.y = best2.y = rand()%15;
        best1.x = best2.x = rand()%15;
    } 
    while(chessboard[best1.y][best1.x] != 0);
    int a1 = score(best1, computer), b1 = score(best1, player);
    for(int i = 0; i < 15; i ++){
        for(int j = 0; j < 15; j ++){
            if(chessboard[i][j] != 0){
                continue;
            }
            point cur = {i, j};
            int m1 = score(cur, computer);
            int m2 = score(cur, player);
            if(m1 > a1){
                best1 = cur;
                a1 = m1;
                b1 = m2; 
            }
            else if(m1 == a1){
                if(m2 > b1){ 
                    best1 = cur;
                    b1 = m2;
                }
            }
        }
    }
    int a2 = score(best2, player), b2 = score(best2, computer);
    for(int i = 0; i < 15; i ++){
        for(int j = 0; j < 15; j ++){
            if(chessboard[i][j] != 0){
                continue;
            }
            point cur = {i, j};
            int m1 = score(cur, player);
            int m2 = score(cur, computer);
            if(m1 > a2){
                best2 = cur;
                a2 = m1;
                b2 = m2;
            }
            else if(m1 == a2){
                if(m2 > b2){
                    best2 = cur;
                    b2 = m2;
                }
            }
        }
    }    
    if(a1 >= a2){
        laozi = best1;
    }
    else{
        laozi = best2;
    }
    chessboard[laozi.y][laozi.x] = computer;
    cout << laozi.y << " " << laozi.x << endl;
    // display();
}
int gobang::score(point p, int who){
    int win5 = 0, alive4 = 0, die4 = 0, ddie4 = 0, alive3 = 0, 
    dalive3 = 0, die3 = 0, alive2 = 0, dalive2 = 0, die2 = 0, nothing = 0;
    int opp;
    if(who == 1){
        opp = 2;
    }
    else{
        opp = 1;
    }
    for(int i = 1; i <= 4; i ++){
        dir d;
        switch(i){
            case 1:
                d = d1;
                break;
            case 2:
                d = d2;
                break;
            case 3:
                d = d3;
                break;
            case 4:
                d = d4;
                break;
       }
       int l = 1;
       point le, ri, p1;
       int left[5], right[5];
       p1 = newPoint(p, d, -1);
       le = p;
       while(isInBoard(p1) && chessboard[p1.y][p1.x] == who){
               le = p1;
               p1 = newPoint(p1, d, -1);
            l ++;
       }
       p1 = newPoint(p, d, 1);
       ri = p;
       while(isInBoard(p1) && chessboard[p1.y][p1.x] == who){
            ri = p1;
            p1 = newPoint(p1, d, 1);
            l ++;
       }
       for(int j = 1; j <= 4; j ++){
               p1 = newPoint(le, d, -j);
               if(isInBoard(p1)){
                   left[j] = chessboard[p1.y][p1.x];
            }
            else{
                left[j] = opp;
            }
               p1 = newPoint(ri, d, j);
               if(isInBoard(p1)){
                   right[j] = chessboard[p1.y][p1.x];
            }
            else{
                right[j] = opp;
            }
       }
       //具体棋型判断
       if(l == 5){
           win5 ++;
       }
       else if(l == 4){
              if(left[1] == 0 && right[1] == 0){//alive4 
                      alive4 ++;
           }
           else if(left[1] == 0 || right[1] == 0){//die4
                   die4 ++;
           }
           else{//nothing
                   nothing ++;
           }
       }
       else if(l == 3){
              if((left[1] == 0 && left[2] == who) || (right[1] == 0 && right[2] == who)){//ddie4
                      ddie4 ++;
           }
           else if(left[1] == 0 && right[1] == 0 && (left[2] == 0 || right[2] == 0)){//alive3
                   alive3 ++;                             
           }
           else if((left[1] == 0 && left[2] == 0) || (right[1] == 0 && right[2] == 0)){//die3
                   die3 ++;
           }
           else if(left[1] == 0 && right[1] == 0){//die3
                   die3 ++; 
           } 
           else{//nothing
                   nothing ++;
           }
       }
       else if(l == 2){
              if((left[1] == 0 && left[2] == who && left[3] == who) && 
                 (right[1] == 0 && right[2] == who && right[3] == who)){//die4
                   ddie4 ++;
           }
           else if(left[1] == 0 && right[1] == 0 && 
                   ((left[2] == who && left[3] == 0) || (right[2] == who && right[3] == 0))){//dalive3
                   dalive3 ++;
           }
           else if((left[1] == 0 && left[3] == 0 && left[2] == who) || 
                   (right[1] == 0 && right[3] == 0 && right[2] == who)){//die3
                die3 ++;
           }
           else if((left[1] == 0 && right[1] == 0) && 
                   (left[2] == who || right[2] == who)){//die3
                   die3 ++;
           }
           else if((left[1] == 0 && left[2] == 0 && left[3] == who) || 
                   (right[1] == 0 && right[2] == 0 && right[3] == who)){//die3
                die3 ++;
           }
           else if((left[1] == 0 && right[1] == 0 && right[2] == 0 && right[3] == 0) || 
                   (left[1] == 0 && left[2] == 0 && right[1] == 0 && right[2] == 0) || 
                   (left[1] == 0 && left[2] == 0 && left[3] == 0 && right[1] == 0)){//alive2
                   alive2 ++;
           }
           else if((left[1] == 0 && left[2] == 0 && left[3] == 0) || 
                   (right[1] == 0 && right[2] == 0 && right[3] == 0)){//die2
                   die2 ++;
           }
           else{//nothing
                   nothing ++;
           }
       }
       else if(l == 1){
              if((left[1] == 0 && left[2] == who && left[3] == who && left[4] == who) || 
                 (right[1] == 0 && right[2] == who && right[3] == who && right[4] == who)){//ddie4
                   ddie4 ++;
           }
           else if((left[1] == 0 && right[1] == 0) && ((left[2] == who && left[3] == who && left[4] == 0) || 
                   (right[2] == who && right[3] == who && right[4] == 0))){//dalive3
                   dalive3 ++;
           }
           else if((left[1] == 0 && right[1] == 0) && 
                   ((left[2] == who && left[3] == who) || (right[2] == who && right[3] == who))){//die3
                   die3 ++;
           }
           else if((left[1] == 0 && left[4] == 0 && left[2] == who && left[3] == who) || 
                   (right[1] == 0 && right[4] == 0 && right[2] == who && right[3] == who)){//die3
                   die3 ++;
           }
           else if((left[1] == 0 && left[2] == 0 && left[3] == who && left[4] == who) || 
                   (right[1] == 0 && right[2] == 0 && right[3] == who && right[4] == who)){//die3
                   die3 ++;
           } 
           else if((left[1] == 0 && left[3] == 0 && left[2] == who && left[4] == who) || 
                   (right[1] == 0 && right[3] == 0 && right[2] == who && right[4] == who)){//die3
                   die3 ++;
           }
           else if((left[1] == 0 && right[1] == 0 && right[3] == 0 && right[2] == who) && (left[2] == 0 || right[4] == 0)){//dalive2
                   dalive2 ++;
           }
           else if((right[1] == 0 && left[1] == 0 && left[3] == 0 && left[2] == who) && 
                   (right[2] == 0 || left[4] == 0)){//dalive2
                   dalive2 ++;
           }
           else if((left[1] == 0 && right[1] == 0 && right[2] == 0 && right[4] == 0 && right[3] == who) || 
                   (right[1] == 0 && left[1] == 0 && left[2] == 0 && left[4] == 0 && left[3] == who)){//dalive2
                   dalive2 ++;
           }
           else if((left[1] == 0 && left[3] == 0 && left[4] == 0 && left[2] == who) || 
                   (right[1] == 0 && right[3] == 0 && right[4] == 0 && right[2] == who)){//die2
                   die2 ++;
           }
           else if((left[1] == 0 && right[1] == 0 && right[2] == 0 && left[2] == who) || 
                   (right[1] == 0 && left[1] == 0 && left[2] == 0 && right[2] == who)){//die2
                   die2 ++;
           }
           else if((left[1] == 0 && left[2] == 0 && left[4] == 0 && left[3] == who) || 
                   (right[1] == 0 && right[2] == 0 && right[4] == 0 && right[3] == who)){//die2
                   die2 ++;
           }
           else if((left[1] == 0 && left[2] == 0 && right[1] == 0 && left[3] == who) || 
                   (right[1] == 0 && right[2] == 0 && left[1] == 0 && right[3] == who)){//die2
                   die2 ++;
           }
           else if((left[1] == 0 && left[2] == 0 && left[3] == 0 && left[4] == who) || 
                   (right[1] == 0 && right[2] == 0 && right[3] == 0 && right[4] == who)){//die2
                   die2 ++;
           }
           else{//nothing
                   nothing ++;
           }
       }
    }
    if (win5 >= 1)
        return 14;//赢5

    if (alive4 >= 1 || die4 >= 2 || (die4 >= 1 && alive3 >= 1))
        return 13;//活4 双死4 死4活3

    if (alive3 >= 2)
        return 12;//双活3

    if (die3 >= 1 && alive3 >= 1)
        return 11;//死3高级活3

    if (die4 >= 1)
        return 10;//高级死4

    if (ddie4 >= 1)
        return 9;//低级死4

    if (alive3 >= 1)
        return 8;//单活3

    if (dalive3 >= 1)
        return 7;//跳活3

    if (alive2 >= 2)
        return 6;//双活2

    if (alive2 >= 1)
        return 5;//活2

    if (dalive2 >= 1)
        return 4;//低级活2

    if (die3 >= 1)
        return 3;//死3

    if (die2 >= 1)
        return 2;//死2

    return 1;//没有威胁
}
bool gobang::isEnd(){
    for(int i = 1; i <= 4; i ++){
        dir d;
        int count = 0;
        switch(i){
            case 1:
                d = d1;
                break;
            case 2:
                d = d2;
                break;
            case 3:
                d = d3;
                break;
            case 4:
                d = d4;
                break;
        }
        for(int j = -4; j <= 4; j ++){
            point p1 = newPoint(laozi, d, j);
            if(isInBoard(p1) && chessboard[p1.y][p1.x] == chessboard[laozi.y][laozi.x]){
                count ++;
            }
            else{
                count = 0;
            }
            if(count == 5){
                return true;
            }
        }
    }
    return false;
}
bool gobang::isInBoard(point p){
    if(p.y >= 0 && p.y < 15 && p.x >= 0 && p.x < 15){
        return true;
    }
    else{
        return false;
    }
}

point gobang::newPoint (point p, dir d, int lenth){
    point p1 = {p.y + d.dy * lenth, p.x + d.dx * lenth};
    return p1;
}

// void gobang::display(){
//     system("cls");
//     for(int i = 0; i < 15; i ++){
//         for(int j = 0; j < 15; j ++){
//             if(i == Y && j == X){
//                 cout << "╋";
//             }
//             else if(chessboard[i][j] == 1){
//                 cout << "○";
//             }
//             else if(chessboard[i][j] == 2){
//                 cout << "●";
//             }
//             else{
//                 cout << ". ";
//             }
                
//         }
//         cout << endl;
//     }
// }

int main(){
    gobang game;
    game.play();
    return 0;
}
