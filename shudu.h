/*
    Shudu类的定义
    所实现的功能：
        生成数独；
        求解数独；
        打印数独
*/
#include <iostream>
#include <string>
#include <string.h>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <streambuf>
#include <map>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <process.h>

using namespace std;

const int N = 9;
typedef vector<vector<char> > Board;

class Shudu
{
private:
    int columnUsed[N];
    int blockUsed[N];
public:
    vector<Board> result;//存放求解的结果终盘
    vector<pair<int, int> > spaces;//存放被挖空的位置坐标
public:
    Shudu()
    {
        initState();
    }

    //位运算 设置i,j位存在gitit
    void flip(int i, int j, int digit);

    void initState();//初始化

    //求解数独
    vector<Board> solveBoard(Board board);

    //DFS进行回溯
    void DFS(Board &board, int pos);

    //打印数独
    void printBoard(Board &board);

    //生成数独终盘，并从中挖去digCount个数
    Board generateBoard(int digCount);

    
    //获取包含0-8的随机数组
    vector<int> getRand9();

    //矩阵变换，将左上角坐标(src_x,src_y)的3x3矩阵进行列扩展或者行扩展。isRow 为是否是行扩展
    void copySquare(Board &board, int src_x, int src_y, bool isRow);
};

