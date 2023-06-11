#include "parse.h"
#include <Windows.h>

int main(int argc, char *argv[]){
    SetConsoleOutputCP(CP_UTF8);
    unordered_map<string, void(*)(int, char*[])> mapOp = {{"-c", print_c}, 
                                                          {"-s", print_s}, 
                                                          {"-n", print_n}, 
                                                          {"-m", print_m},
                                                          {"-r", print_r},
                                                          {"-u", print_u}};
    if (argc < 2) {
        cout << "参数错误，argc = " << argc << endl;
        return -1;
    } 
    string op = argv[1];
    if (mapOp.find(op) != mapOp.end()) {
        mapOp[op](argc, argv);
    }
    return 0;
}