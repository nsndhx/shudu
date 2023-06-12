/*
    实现parse.h中定义的函数
*/
#include "parse.h"
#include "getopt.h"         //我们利用一下IBM的命令行输入框架来进行输入的解释
#pragma warning(disable : 4996)

// 需要的数独终盘数量
void print_c(int argc, char *argv[])
{
    Shudu player;
    vector<int> range;
    ofstream outfile;

    int num = atoi(argv[2]);
    if (num < 1 || num > 1000000)
    {
        printf("生成数独终盘数量范围在1~1000000之间\n");
        exit(0);
    }
    outfile.open("game.txt", ios::out | ios::trunc);
    range.push_back(0);
    generateGame(atoi(argv[2]), 0, range, outfile, player);
    range.clear();
}

// 需要解的数独棋盘文件路径
void print_s(int argc, char *argv[])
{

}

// 需要的游戏数量
void print_n(int argc, char *argv[])
{

}

// 生成游戏的难度
void print_m(int argc, char *argv[])
{
    printf("没有此命令，请检查命令（m只可与n同时使用）");
    exit(0);
}

// 生成游戏中挖空的数量范围
void print_r(int argc, char *argv[])
{
    printf("没有此命令，请检查命令（r只可与n同时使用）");
    exit(0);
}

// 打印游戏的解唯一
void print_u(int argc, char *argv[])
{
    printf("没有此命令，请检查命令（u只可与n同时使用）");
    exit(0);
}
