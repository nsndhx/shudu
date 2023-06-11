#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>

#include "head.h"

using namespace std;

int result[100000000];//用于存放指向 存放数独终局数据数组的指针
int solve_map[1000005][81];//用于存放求解数独时的数独问题

class sudo
{
private:
	int sudo_map[9][9];
	int count;
	int num;
public:
	sudo() {};
	sudo(int n)
	{
		count = 0;
		num = n;

	}
	void creat_Que();//生成数独终局
	void write_to_file();//将数据写入文件
	
	void solve_Que(int count,int que_num);//求解数独终局
	bool is_right(int count, int que_num);//判断填入数据的合法性
};

void sudo::creat_Que()//生成数独终局
{

}


bool sudo::is_right(int count, int que_num)//判断填入数据的合法性
{

}


void sudo::solve_Que(int count,int que_num)//求解数独终局
{


}

void sudo::write_to_file()//将数据写入文件
{

}

int main(int argc, char const *argv[])
{
	
}