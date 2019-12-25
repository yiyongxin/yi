/***************************************
* brief:    入口函数
* author:   易雍鑫
* date:     2018-04-08
****************************************/

#include "../common/log/log_ifs.h"
#include "../test/test.h"


int main(int argc, char *argv[])
{
    initLog();
    
    test_src();
    
    closeLog();
    
    return 1;
} 