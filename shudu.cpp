/*
    实现Shudu类中的函数
*/
#include "shudu.h"

void Shudu::initState(){
    memset(rowUsed, 0, sizeof(rowUsed));
    memset(columnUsed, 0, sizeof(columnUsed));
    memset(blockUsed, 0, sizeof(blockUsed));
    spaces.clear();
    result.clear();
}

void Shudu::addResult(Board &board){
    vector<vector<char> > obj(board);
    result.push_back(obj);
}

//位运算 设置i,j位存在gitit
void Shudu::flip(int i, int j, int digit){
    rowUsed[i] ^= (1 << digit);
    columnUsed[j] ^= (1 << digit);
    blockUsed[(i / 3) * 3 + j / 3] ^= (1 << digit);
}

//求解数独
vector<Board> Shudu::solveBoard(Board board){
    
    initState();
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            //数独的空位用'$'表示
            if (board[i][j] == '$'){
                spaces.push_back(pair<int, int>(i, j));
            }
            else{
                int digit = board[i][j] - '1';
                //数独终盘的i,j坐标存在digit数
                flip(i, j, digit);
            }
        }
    }
    DFS(board, 0);
    return result;
}



//DFS进行回溯
void Shudu::DFS(Board &board, int pos){
    if (pos == spaces.size()){
        addResult(board);
        return;
    }
    int i = spaces[pos].first;
    int j = spaces[pos].second;
    //mask 能查询出 i,j 的位置不能放哪些数
    int mask = ~(rowUsed[i] | columnUsed[j] | blockUsed[(i / 3) * 3 + j / 3]) & 0x1ff;
    int digit = 0;
    while (mask){
        if (mask & 1){
            flip(i, j, digit);
            board[i][j] = '1' + digit;
            DFS(board, pos + 1);
            flip(i, j, digit);
        }
        mask = mask >> 1;
        digit++;
    }
}

//打印数独
void Shudu::printBoard(Board &board){
    for (int i = 0; i < board.size(); i++){
        for (int j = 0; j < board[i].size(); j++){
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
}

//生成数独终盘，并从中挖去digCount个数
Board Shudu::generateBoard(int digCount){
    vector<vector<char> > board(N, vector<char>(N, '$'));
    //生成0-8的数，位置随机
    vector<int> row = getRand9();

    //初始化中间的3*3矩阵
    for (int i = 0; i < 3; i++){
        board[3][i + 3] = row[i] + '1';
        board[4][i + 3] = row[i + 3] + '1';
        board[5][i + 3] = row[i + 6] + '1';
    }

    //矩阵变换以填充其余8个矩阵
    copySquare(board, 3, 3, true);
    copySquare(board, 3, 3, false);
    copySquare(board, 3, 0, false);
    copySquare(board, 3, 6, false);

    //挖取操作
    while (digCount){
        int x = rand() % 9;
        int y = rand() % 9;
        if (board[x][y] == '$')
            continue;
        char tmp = board[x][y];
        board[x][y] = '$';
        //检验解是否唯一
        solveBoard(board);
        if (result.size() == 1)
            digCount--;
        else
            board[x][y] = tmp;
        digCount--;
    }

    return board;
}

//获取包含0-8的随机数组
vector<int> Shudu::getRand9(){
    vector<int> result;
    int digit = 0;
    while (result.size() != 9){
        int num = rand() % 9;
        if ((1 << num) & digit)
            continue;
        else{
            result.push_back(num);
            digit ^= (1 << num);
        }
    }
    return result;
}

//矩阵变换，将左上角坐标(src_x,src_y)的3x3矩阵进行列扩展或者行扩展。isRow 为是否是行扩展
void Shudu::copySquare(Board &board, int src_x, int src_y, bool isRow){
    int rand_tmp = rand() % 2 + 1;
    int order_first[3] = {1, 2, 0};
    int order_second[3] = {2, 0, 1};
    if (rand_tmp == 2){
        order_first[0] = 2;
        order_first[1] = 0;
        order_first[2] = 1;
        order_second[0] = 1;
        order_second[1] = 2;
        order_second[2] = 0;
    }
    for (int i = 0; i < 3; i++){
        if (isRow){
            board[src_x][i] = board[src_x + order_first[0]][src_y + i];
            board[src_x + 1][i] = board[src_x + order_first[1]][src_y + i];
            board[src_x + 2][i] = board[src_x + order_first[2]][src_y + i];
            board[src_x][i + 6] = board[src_x + order_second[0]][src_y + i];
            board[src_x + 1][i + 6] = board[src_x + order_second[1]][src_y + i];
            board[src_x + 2][i + 6] = board[src_x + order_second[2]][src_y + i];
        }
        else{
            board[i][src_y] = board[src_x + i][src_y + order_first[0]];
            board[i][src_y + 1] = board[src_x + i][src_y + order_first[1]];
            board[i][src_y + 2] = board[src_x + i][src_y + order_first[2]];
            board[i + 6][src_y] = board[src_x + i][src_y + order_second[0]];
            board[i + 6][src_y + 1] = board[src_x + i][src_y + order_second[1]];
            board[i + 6][src_y + 2] = board[src_x + i][src_y + order_second[2]];
        }
    }
}

//测试用
int main(){
    Shudu player;
    Board a = player.generateBoard(30);
    player.printBoard(a);
    for(int i = 0;i<10;i++){
        cout<<"round "<<i+1<<endl;
        Board a = player.generateBoard(30);
        player.printBoard(a);
        cout << endl;
        player.solveBoard(a);
        int k=0;
        while(k < player.result.size()){
            cout<<"result "<<k+1<<endl;
            player.printBoard(player.result[k]);
            cout<<endl;
            k++;
        }
    }
    system("pause");
    return 0;
}