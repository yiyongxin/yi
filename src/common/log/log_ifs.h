/***************************************
* brief:    日志功能宏接口
* author:   易雍鑫
* date:     2018-03-14
****************************************/

#ifdef _LOG4CPLUS
    #include "log_ifs_log4clpus.h"
#else
    #include<iostream>
    
    #define LOGIFS_ERR(str)
    #define LOGIFS_INFO(str)
    #define LOGIFS_DEBUG(str)
    #define LOGIFS_WARN(str)
    #define LOGIFS_FATAL(str)

#endif