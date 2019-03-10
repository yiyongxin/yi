/***************************************
* brief:    日志功能宏接口
* author:   易雍鑫
* date:     2018-03-14
****************************************/
#ifndef LOG_IFS_H_
#define LOG_IFS_H_

#ifdef _LOG4CPLUS
    #include "log_log4clpus_ifs.h"
#else
    #include<iostream>
    
    #define LOGIFS_ERR(str)
    #define LOGIFS_INFO(str)
    #define LOGIFS_DEBUG(str)
    #define LOGIFS_WARN(str)
    #define LOGIFS_FATAL(str)

#endif

#endif //LOG_IFS_H_
