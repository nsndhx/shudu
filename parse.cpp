/*
    实现parse.h中定义的函数
*/
#include "parse.h"
#include "getopt.h"
#pragma warning(disable:4996)


// 需要的数独终盘数量
void print_c(int argc, char* argv[])
{

}

// 需要解的数独棋盘文件路径
void print_s(int argc, char* argv[])
{

}

// 需要的游戏数量
void print_n(int argc, char* argv[])
{

}

// 生成游戏的难度
void print_m(int argc, char* argv[])
{
    printf("没有此命令，请检查命令（-m只可与-n同时使用）");
    exit(0);
}

// 生成游戏中挖空的数量范围
void print_r(int argc, char* argv[])
{
    printf("没有此命令，请检查命令（-r只可与-n同时使用）");
    exit(0);
}

// 打印游戏的解唯一
void print_u(int argc, char* argv[])
{
    printf("没有此命令，请检查命令（-u只可与-n同时使用）");
    exit(0);
}