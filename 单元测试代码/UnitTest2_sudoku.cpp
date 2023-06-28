#include "pch.h"
#include "CppUnitTest.h"
#include "../test_shudu/shudu.h"
#include "../test_shudu/helpFunc.h"
#include "../test_shudu/parse.h"

#pragma warning(disable:4996)



//由于出现了一些链接上的问题，我们这里再次给出一部分函数的定义

//getopt
#include <stdio.h>                  /* for EOF */
#include <string.h>                 /* for strchr() */

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
  /* static (global) variables that are specified as exported by getopt() */
char* optarg = NULL;    /* pointer to the start of the option argument  */
int   optind = 1;       /* number of the next argv[] to be evaluated    */
int   opterr = 1;       /* non-zero if a question mark should be returned
                           when a non-valid option character is detected */

                           /* handle possible future character set concerns by putting this in a macro */
#define _next_char(string)  (char)(*(string+1))

int getopt(int argc, char* argv[], char* opstring)
{
    static char* pIndexPosition = NULL; /* place inside current argv string */
    char* pArgString = NULL;        /* where to start from next */
    char* pOptString;               /* the string in our program */


    if (pIndexPosition != NULL) {
        /* we last left off inside an argv string */
        if (*(++pIndexPosition)) {
            /* there is more to come in the most recent argv */
            pArgString = pIndexPosition;
        }
    }

    if (pArgString == NULL) {
        /* we didn't leave off in the middle of an argv string */
        if (optind >= argc) {
            /* more command-line arguments than the argument count */
            pIndexPosition = NULL;  /* not in the middle of anything */
            return EOF;             /* used up all command-line arguments */
        }

        /*---------------------------------------------------------------------
         * If the next argv[] is not an option, there can be no more options.
         *-------------------------------------------------------------------*/
        pArgString = argv[optind++]; /* set this to the next argument ptr */

        if (('/' != *pArgString) && /* doesn't start with a slash or a dash? */
            ('-' != *pArgString)) {
            --optind;               /* point to current arg once we're done */
            optarg = NULL;          /* no argument follows the option */
            pIndexPosition = NULL;  /* not in the middle of anything */
            return EOF;             /* used up all the command-line flags */
        }

        /* check for special end-of-flags markers */
        if ((strcmp(pArgString, "-") == 0) ||
            (strcmp(pArgString, "--") == 0)) {
            optarg = NULL;          /* no argument follows the option */
            pIndexPosition = NULL;  /* not in the middle of anything */
            return EOF;             /* encountered the special flag */
        }

        pArgString++;               /* look past the / or - */
    }

    if (':' == *pArgString) {       /* is it a colon? */
        /*---------------------------------------------------------------------
         * Rare case: if opterr is non-zero, return a question mark;
         * otherwise, just return the colon we're on.
         *-------------------------------------------------------------------*/
        return (opterr ? (int)'?' : (int)':');
    }
    else if ((pOptString = strchr(opstring, *pArgString)) == 0) {
        /*---------------------------------------------------------------------
         * The letter on the command-line wasn't any good.
         *-------------------------------------------------------------------*/
        optarg = NULL;              /* no argument follows the option */
        pIndexPosition = NULL;      /* not in the middle of anything */
        return (opterr ? (int)'?' : (int)*pArgString);
    }
    else {
        /*---------------------------------------------------------------------
         * The letter on the command-line matches one we expect to see
         *-------------------------------------------------------------------*/
        if (':' == _next_char(pOptString)) { /* is the next letter a colon? */
            /* It is a colon.  Look for an argument string. */
            if ('\0' != _next_char(pArgString)) {  /* argument in this argv? */
                optarg = &pArgString[1];   /* Yes, it is */
            }
            else {
                /*-------------------------------------------------------------
                 * The argument string must be in the next argv.
                 * But, what if there is none (bad input from the user)?
                 * In that case, return the letter, and optarg as NULL.
                 *-----------------------------------------------------------*/
                if (optind < argc)
                    optarg = argv[optind++];
                else {
                    optarg = NULL;
                    return (opterr ? (int)'?' : (int)*pArgString);
                }
            }
            pIndexPosition = NULL;  /* not in the middle of anything */
        }
        else {
            /* it's not a colon, so just return the letter */
            optarg = NULL;          /* no argument follows the option */
            pIndexPosition = pArgString;    /* point to the letter we're on */
        }
        return (int)*pArgString;    /* return the letter that matched */
    }
}

//parse.cpp

map<char, string> parse(int argc, char* argv[])
{
    map<char, string> params;
    int  gameNumber, gameLevel;
    vector<int> range;
    string inputFile;
    char opt = 0;
    while ((opt = getopt(argc, argv, (char*)"c:s:n:m:r:u")) != -1)
    {
        switch (opt)
        {
        case 'c':
        case 's':
            printf("Sth has been wrong");
            break;
        case 'n':
            gameNumber = atoi(optarg);
            if (gameNumber < 1 || gameNumber > 10000)
            {
                printf("生成数独游戏数量范围在1~10000之间\n");
                exit(0);
            }
            params[opt] = string(optarg);
            break;
        case 'm':
            gameLevel = atoi(optarg);
            if (gameLevel < 1 || gameLevel > 3)
            {
                printf("生成游戏难度的范围在1~3之间\n");
                exit(0);
            }
            params[opt] = string(optarg);
            break;
        case 'r':
            char* p;
            p = strtok(optarg, "~");
            while (p)
            {
                range.push_back(atoi(p));
                p = strtok(NULL, "~");
            }
            if (range.size() != 2)
            {
                printf("请输入一个范围参数\n");
                exit(0);
            }
            if ((range[0] >= range[1]) || range[0] < 20 || range[1] > 55)
            {
                printf("请输入合法范围20~55\n");
                exit(0);
            }
            params[opt] = string(optarg);
            break;
        case 'u':
            params[opt] = string();
            break;
        default:
            printf("请输入合法参数\n");
            exit(0);
            break;
        }
    }
    return params;
}

// 需要的数独终盘数量
void print_c(int argc, char* argv[])
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
void print_s(int argc, char* argv[])
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
        vector<Board> result = player.solveSudoku(boards[i]);
        writeFile(result, outfile);
    }
    outfile.close();
}

// 需要的游戏数量
void print_n(int argc, char* argv[])
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
void print_m(int argc, char* argv[])
{
    printf("没有此命令，请检查命令（m只可与n同时使用）\n");
    exit(0);
}

// 生成游戏中挖空的数量范围
void print_r(int argc, char* argv[])
{
    printf("没有此命令，请检查命令（r只可与n同时使用）\n");
    exit(0);
}

// 打印游戏的解唯一
void print_u(int argc, char* argv[])
{
    printf("没有此命令，请检查命令（u只可与n同时使用）\n");
    exit(0);
}

vector<Board> readFile(string filePath)
{
    ifstream infile;
    vector<Board> boards;
    infile.open(filePath);
    char data[100];
    Board tmp;
    vector<char> row;
    while (!infile.eof())
    {
        infile.getline(data, 100);
        if (data[0] == '-')
        {
            boards.push_back(Board(tmp));
            tmp.clear();
            continue;
        }
        for (int i = 0; i < strlen(data); i++)
        {
            if (('1' <= data[i] && data[i] <= '9') || data[i] == '$')
            {
                row.push_back(data[i]);
            }
        }
        tmp.push_back(vector<char>(row));
        row.clear();
    }
    infile.close();
    return boards;
}

void writeFile(vector<Board> boards, ofstream& f)
{
    for (int k = 0; k < boards.size(); k++)
    {
        for (int i = 0; i < boards[k].size(); i++)
        {
            for (int j = 0; j < boards[k][i].size(); j++)
            {
                f << boards[k][i][j] << " ";
            }
            f << "\n";
        }
        f << "------- " << k << " -------" << endl;
    }
}

void generateGame(int gameNumber, int gameLevel, vector<int> digCount, ofstream& outfile, Shudu& player)
{
    for (int i = 0; i < gameNumber; i++)
    {
        int cnt = 0;
        if (digCount.size() == 1)
        {
            cnt = digCount[0];
        }
        else
        {
            cnt = rand() % (digCount[1] - digCount[0] + 1) + digCount[0];
        }
        Board b = player.generateBoard(cnt);
        vector<Board> bs;
        bs.push_back(b);
        writeFile(bs, outfile);
    }
    outfile.close();
}

void Shudu::initState() {
    memset(rowUsed, 0, sizeof(rowUsed));
    memset(columnUsed, 0, sizeof(columnUsed));
    memset(blockUsed, 0, sizeof(blockUsed));
    spaces.clear();
    result.clear();
}

void Shudu::addResult(Board& board) {
    vector<vector<char> > obj(board);
    result.push_back(obj);
}

void Shudu::flip(int i, int j, int digit) {
    rowUsed[i] ^= (1 << digit);
    columnUsed[j] ^= (1 << digit);
    blockUsed[(i / 3) * 3 + j / 3] ^= (1 << digit);
}

vector<Board> Shudu::solveSudoku(Board board) {
    initState();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            //数独的空位用'$'表示
            if (board[i][j] == '$') {
                spaces.push_back(pair<int, int>(i, j));
            }
            else {
                int digit = board[i][j] - '1';
                //数独终盘的i,j坐标存在digit数
                flip(i, j, digit);
            }
        }
    }
    DFS(board, 0);
    return result;
}

void Shudu::DFS(Board& board, int pos) {
    if (pos == spaces.size()) {
        addResult(board);
        return;
    }
    int i = spaces[pos].first;
    int j = spaces[pos].second;
    //mask 能查询出 i,j 的位置不能放哪些数
    int mask = ~(rowUsed[i] | columnUsed[j] | blockUsed[(i / 3) * 3 + j / 3]) & 0x1ff;
    int digit = 0;
    while (mask) {
        if (mask & 1) {
            flip(i, j, digit);
            board[i][j] = '1' + digit;
            DFS(board, pos + 1);
            flip(i, j, digit);
        }
        mask = mask >> 1;
        digit++;
    }
}

void Shudu::getResult() {
    for (size_t i = 0; i < result.size(); i++) {
        Board board = result[i];
        printBoard(board);
    }
}

bool Shudu::checkBoard(Board& board) {
    initState();
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != '$') {
                int digit = board[i][j] - '1';
                if ((rowUsed[i] | columnUsed[j] | blockUsed[(i / 3) * 3 + j / 3]) & (1 << digit)) {
                    return false;
                }
                flip(i, j, digit);
            }
        }
    }
    return true;
}

void Shudu::printBoard(Board& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
}

Board Shudu::generateBoard(int digCount) {
    vector<vector<char> > board(N, vector<char>(N, '$'));
    //生成0-8的数，位置随机
    vector<int> row = getRand9();
    for (int i = 0; i < 3; i++) {
        board[3][i + 3] = row[i] + '1';
        board[4][i + 3] = row[i + 3] + '1';
        board[5][i + 3] = row[i + 6] + '1';
    }
    //以下四个步骤就是上面的矩阵变换
    copySquare(board, 3, 3, true);
    copySquare(board, 3, 3, false);
    copySquare(board, 3, 0, false);
    copySquare(board, 3, 6, false);

    //挖取操作
    while (digCount) {
        int x = rand() % 9;
        int y = rand() % 9;
        if (board[x][y] == '$')
            continue;
        char tmp = board[x][y];
        board[x][y] = '$';

        solveSudoku(board);
        if (result.size() == 1)
            digCount--;
        else
            board[x][y] = tmp;
    }
    if (!checkBoard(board))
        cout << "wrong board" << endl;
    return board;
}

vector<int> Shudu::getRand9() {
    vector<int> result;
    int digit = 0;
    while (result.size() != 9) {
        int num = rand() % 9;
        if ((1 << num) & digit)
            continue;
        else {
            result.push_back(num);
            digit ^= (1 << num);
        }
    }
    return result;
}

void Shudu::copySquare(Board& board, int src_x, int src_y, bool isRow) {
    int rand_tmp = rand() % 2 + 1;
    int order_first[3] = { 1, 2, 0 };
    int order_second[3] = { 2, 0, 1 };
    if (rand_tmp == 2) {
        order_first[0] = 2;
        order_first[1] = 0;
        order_first[2] = 1;
        order_second[0] = 1;
        order_second[1] = 2;
        order_second[2] = 0;
    }
    for (int i = 0; i < 3; i++) {
        if (isRow) {
            board[src_x][i] = board[src_x + order_first[0]][src_y + i];
            board[src_x + 1][i] = board[src_x + order_first[1]][src_y + i];
            board[src_x + 2][i] = board[src_x + order_first[2]][src_y + i];
            board[src_x][i + 6] = board[src_x + order_second[0]][src_y + i];
            board[src_x + 1][i + 6] = board[src_x + order_second[1]][src_y + i];
            board[src_x + 2][i + 6] = board[src_x + order_second[2]][src_y + i];
        }
        else {
            board[i][src_y] = board[src_x + i][src_y + order_first[0]];
            board[i][src_y + 1] = board[src_x + i][src_y + order_first[1]];
            board[i][src_y + 2] = board[src_x + i][src_y + order_first[2]];
            board[i + 6][src_y] = board[src_x + i][src_y + order_second[0]];
            board[i + 6][src_y + 1] = board[src_x + i][src_y + order_second[1]];
            board[i + 6][src_y + 2] = board[src_x + i][src_y + order_second[2]];
        }
    }
}


/*
const char data[9][9] = {
    {'5', '3', '$', '$', '7', '$', '$', '$', '$'},
    {'6', '$', '$', '1', '9', '5', '$', '$', '$'},
    {'$', '9', '8', '$', '$', '$', '$', '6', '$'},
    {'8', '$', '$', '$', '6', '$', '$', '$', '3'},
    {'4', '$', '$', '8', '$', '3', '$', '$', '1'},
    {'7', '$', '$', '$', '2', '$', '$', '$', '6'},
    {'$', '6', '$', '$', '$', '$', '2', '8', '$'},
    {'$', '$', '$', '4', '1', '9', '$', '$', '5'},
    {'$', '$', '$', '$', '8', '$', '$', '7', '9'}
};*/

namespace UnitTest2sudoku
{
	TEST_CLASS(UnitTest2sudoku)
	{
	public:
		
        //这个测试说明能够正确的检测出正确的棋盘
		TEST_METHOD(TestMethod_isBoardValid)
		{

                Shudu player;
                vector<vector<char> > board(N, vector<char>(N, '.'));

                for (int i = 0; i < board.size(); i++)
                {
                    for (int j = 0; j < board[i].size(); j++)
                    {
                        board[i][j] = ::data[i][j];
                    }
                }
                bool isBoardValid = player.checkBoard(board);

                Assert::IsTrue(isBoardValid, L"棋盘无效。");
		}

        //这个测试说明能够正确的识别错误棋盘
        TEST_METHOD(Board_notValid){
            Shudu player;

            vector<vector<char> > board(N, vector<char>(N, '.'));

            char invaild_data[9][9] = {
            {'5', '3', '$', '$', '3', '$', '$', '$', '$'},
            {'6', '$', '$', '1', '9', '5', '$', '$', '$'},
            {'$', '9', '8', '$', '$', '$', '$', '6', '$'},
            {'8', '$', '$', '$', '6', '$', '$', '$', '3'},
            {'4', '$', '$', '8', '3', '3', '$', '$', '1'},
            {'7', '$', '$', '$', '2', '$', '$', '$', '6'},
            {'$', '6', '$', '$', '$', '$', '2', '8', '$'},
            {'$', '$', '$', '4', '1', '9', '$', '$', '5'},
            {'$', '$', '$', '$', '3', '$', '$', '7', '9'}
                    };

            for (int i = 0; i < board.size(); i++)
            {
                for (int j = 0; j < board[i].size(); j++)
                {
                    board[i][j] = invaild_data[i][j];
                }
            }
            bool isBoardValid = player.checkBoard(board);

            Assert::IsTrue(isBoardValid, L"棋盘无效。");
        }

        //这个测试说明能够正确的产生棋盘的解
        TEST_METHOD(valid_solution) {
            Shudu player;
            vector<vector<char> > unsolved_board(N, vector<char>(N, '.'));

            //这个棋盘的解唯一
            char unsolved[9][9]= {
            {'9', '7', '$', '1', '2', '6', '5', '4', '3'},
            {'3', '5', '4', '$', '7', '8', '$', '6', '$'},
            {'1', '2', '$', '3', '5', '4', '$', '8', '9'},
            {'8', '$', '7', '$', '1', '$', '4', '3', '$'},
            {'4', '3', '5', '8', '9', '7', '6', '1', '$'},
            {'6', '1', '2', '$', '$', '5', '8', '$', '7'},
            {'7', '$', '$', '2', '$', '$', '3', '5', '$'},
            {'5', '4', '$', '$', '8', '9', '$', '$', '6'},
            {'$', '6', '$', '$', '$', '$', '9', '$', '8'}
            };
            for (int i = 0; i < unsolved_board.size(); i++)
            {
                for (int j = 0; j < unsolved_board[i].size(); j++)
                {
                    unsolved_board[i][j] = unsolved[i][j];
                }
            }

            vector<vector<vector<char> >> solved_board = player.solveSudoku(unsolved_board);

            char result[9][9]= {
            {'9', '7', '8', '1', '2', '6', '5', '4', '3'},
            {'3', '5', '4', '9', '7', '8', '2', '6', '1'},
            {'1', '2', '6', '3', '5', '4', '7', '8', '9'},
            {'8', '9', '7', '6', '1', '2', '4', '3', '5'},
            {'4', '3', '5', '8', '9', '7', '6', '1', '2'},
            {'6', '1', '2', '4', '3', '5', '8', '9', '7'},
            {'7', '8', '9', '2', '6', '1', '3', '5', '4'},
            {'5', '4', '3', '7', '8', '9', '1', '2', '6'},
            {'2', '6', '1', '5', '4', '3', '9', '7', '8'}
            };

            vector<vector<char> > answer_board(N, vector<char>(N, '.'));

            for (int i = 0; i < answer_board.size(); i++)
            {
                for (int j = 0; j < answer_board[i].size(); j++)
                {
                    answer_board[i][j] = result[i][j];
                }
            }

            bool isRight = true;

            for (int i = 0; i < answer_board.size(); i++)
            {
                for (int j = 0; j < answer_board[i].size(); j++)
                {
                    if(answer_board[i][j] != solved_board[0][i][j])
                        isRight = false;
                }
            }

            Assert::IsTrue(isRight, L"求解错误");

        }


        //这个测试说明能正确的求出解的个数---1的情况
        TEST_METHOD(solution_number_1) {
            Shudu player;
            vector<vector<char> > unsolved_board(N, vector<char>(N, '.'));

            //这个棋盘的解唯一
            char unsolved[9][9] = {
            {'9', '7', '$', '1', '2', '6', '5', '4', '3'},
            {'3', '5', '4', '$', '7', '8', '$', '6', '$'},
            {'1', '2', '$', '3', '5', '4', '$', '8', '9'},
            {'8', '$', '7', '$', '1', '$', '4', '3', '$'},
            {'4', '3', '5', '8', '9', '7', '6', '1', '$'},
            {'6', '1', '2', '$', '$', '5', '8', '$', '7'},
            {'7', '$', '$', '2', '$', '$', '3', '5', '$'},
            {'5', '4', '$', '$', '8', '9', '$', '$', '6'},
            {'$', '6', '$', '$', '$', '$', '9', '$', '8'}
            };
            for (int i = 0; i < unsolved_board.size(); i++)
            {
                for (int j = 0; j < unsolved_board[i].size(); j++)
                {
                    unsolved_board[i][j] = unsolved[i][j];
                }
            }

            vector<vector<vector<char> >> solved_board = player.solveSudoku(unsolved_board);

            int solution = 1;//解的个数

            Assert::AreEqual(solution, (int)solved_board.size());

        }
        //这个测试说明我们实现的类中的方法能够给出一个正确的棋盘
        TEST_METHOD(GenerateValidBoard) {
            Shudu player;
            Board newBoard = player.generateBoard(25);
            bool isValid = player.checkBoard(newBoard);
            Assert::IsTrue(isValid, L"棋盘无效。");
        }


	};
}
