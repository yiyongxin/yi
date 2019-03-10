/***************************************
* brief:    入口函数
* author:   易雍鑫
* date:     2018-04-08
****************************************/

#include "../common/log/log.hpp"
#include "../common/log/log_ifs.h"
#include "../test/test.h"

int main(int argc, char *argv[])
{
    initLog();
    LOG4CPLUS_INFO(_logger_info,"start log");
    test_src();
    log4cplus::Logger::shutdown();
    return 1;
}