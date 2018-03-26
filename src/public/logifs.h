/*************************************** 
* brief:    日志功能宏接口
* author:   易雍鑫
* date:     2018-03-14 
****************************************/ 

#define _LOG4CPLUS
#ifdef _LOG4CPLUS
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>

extern log4cplus::Logger _logger_err;
extern log4cplus::Logger _logger_debug;
extern log4cplus::Logger _logger_info;
extern log4cplus::Logger _logger_warn;
extern log4cplus::Logger _logger_fatal;

#define LOGIFS_ERR(str) 1//LOG4CPLUS_ERROR(_logger_err,str)

#define LOGIFS_INFO(str) 1

#define LOGIFS_DEBUG(str) 1//LOG4CPLUS_DEBUG(_logger_debug,str);

#define LOGIFS_WARN(str) 1

#define LOGIFS_FATAL(str) 1

#endif