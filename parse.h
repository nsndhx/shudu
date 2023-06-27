/*
    该函数用来处理读入的命令，并对应进行功能的实现
*/

#ifndef HELPFUNC_H
#define HELPFUNC_H

#include "helpFunc.h"

map<char, string> parse(int argc, char *argv[]);//解析输入参数

void print_c(int argc, char *argv[]);//需要的数独终盘数量
void print_s(int argc, char *argv[]);//需要解的数独棋盘文件路径
void print_n(int argc, char *argv[]);//需要的游戏数量
void print_m(int argc, char *argv[]);//生成游戏的难度
void print_r(int argc, char *argv[]);//生成游戏中挖空的数量范围
void print_u(int argc, char *argv[]);//打印游戏的解唯一
#endif // PARSE_H
