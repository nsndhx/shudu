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
    Shudu player;
    ofstream outfile;
    vector<Board> boards;

    if (access(argv[2], 0) == -1)
    {
        printf("file does not exist\n");
        exit(0);
    }
    outfile.open("sudoku.txt", ios::out | ios::trunc);
    string filePath = argv[2];
    boards = readFile(filePath);
    for (int i = 0; i < boards.size(); i++)
    {
        vector<Board> result = player.solveBoard(boards[i]);
        writeFile(result, outfile);
    }
    outfile.close();
}

// 需要的游戏数量
void print_n(int argc, char *argv[])
{
    Shudu player;
    map<char, string> params = parse(argc, argv);
    map<char, string>::iterator it, tmp;
    vector<int> range;
    ofstream outfile;
    int gameNumber;
    int gameLevel = 0;
    int solution_count = 0;
    it = params.begin();
    while (it != params.end())
    {
        tmp = params.find('n');
        if (tmp == params.end())
        {
            printf("缺少参数 n \n");
            exit(0);
        }

        gameNumber = atoi(tmp->second.c_str());

        tmp = params.find('u');
        if (tmp != params.end())
        {
            solution_count = 1;
        }

        tmp = params.find('m');
        if (tmp != params.end())
        {
            gameLevel = atoi(tmp->second.c_str());
        }

        tmp = params.find('r');
        if (tmp != params.end())
        {
            char* p;
            char* pc = new char[100];
            strcpy(pc, tmp->second.c_str());
            p = strtok(pc, "~");
            while (p)
            {
                range.push_back(atoi(p));
                p = strtok(NULL, "~");
            }
        }
        else
        {
            // 根据不同级别采取挖空数量不同
            if (gameLevel == 1)
            {
                range.push_back(20);
                range.push_back(30);
            }
            else if (gameLevel == 2)
            {
                range.push_back(30);
                range.push_back(40);
            }
            else if (gameLevel == 3)
            {
                range.push_back(40);
                range.push_back(55);
            }
            else
            {
                range.push_back(20);
                range.push_back(55);
            }
        }
        outfile.open("game.txt", ios::out | ios::trunc);
        generateGame(gameNumber, gameLevel, range, outfile, player);
        range.clear();
        // cout << it->first << ' ' << it->second << endl;
        it++;
    }
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
