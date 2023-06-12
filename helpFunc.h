/*
    该文件用来存储实现功能所需的函数
    实现的功能：
        读取文件；
        写入文件
*/
#include "Shudu.h"

vector<Board> readFile(string filePath);
void writeFile(vector<Board> boards, ofstream &f);
void generateGame(int gameNumber, int gameLevel, vector<int> digCount, ofstream &outfile, Shudu &player);
