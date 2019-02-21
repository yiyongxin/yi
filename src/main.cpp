/***************************************
* brief:    入口函数
* author:   易雍鑫
* date:     2018-04-08
****************************************/

// #include <stdlib.h>  
// #include <stdio.h>

#include "public/log.hpp"
#include "public/logifs.h"
#include "test_src/test.h"

int main(int argc, char *argv[])
{
    initLog();
    LOG4CPLUS_INFO(_logger_info,"start log");
    test_src();
    log4cplus::Logger::shutdown();
    return 1;
}