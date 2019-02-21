/***************************************
* brief:    日志功能宏接口
* author:   易雍鑫
* date:     2018-03-14
****************************************/

#ifdef _LOG4CPLUS
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>  

extern log4cplus::Logger _logger_err;
extern log4cplus::Logger _logger_debug;
extern log4cplus::Logger _logger_info;
extern log4cplus::Logger _logger_warn;
extern log4cplus::Logger _logger_fatal;

#define LOGIFS_ERR(str)\
     {\
        LOG4CPLUS_ERROR(_logger_err,__FILE__ << __LINE__ << __FUNCTION__);\
        LOG4CPLUS_ERROR(_logger_err,str);\
    }

#define LOGIFS_INFO(str)\
    {\
        LOG4CPLUS_INFO(_logger_info,__FILE__ << __LINE__ << __FUNCTION__);\
        LOG4CPLUS_INFO(_logger_info,str);\
    }

#define LOGIFS_DEBUG(str)\
    {\
        LOG4CPLUS_DEBUG(_logger_debug,__FILE__ << __LINE__ << __FUNCTION__);\
        LOG4CPLUS_DEBUG(_logger_debug,str);\
    }

#define LOGIFS_WARN(str)\
    {\
        LOG4CPLUS_WARN(_logger_warn,__FILE__ << __LINE__ << __FUNCTION__);\
        LOG4CPLUS_WARN(_logger_warn,str);\
    }

#define LOGIFS_FATAL(str)\
    {\
        LOG4CPLUS_FATAL(_logger_fatal,__FILE__ << __LINE__ << __FUNCTION__);\
        LOG4CPLUS_FATAL(_logger_fatal,str);\
    }

#else
// #include<iostream>

// #define LOGIFS_ERR(str)
// #define LOGIFS_INFO(str) //   std::cout << str;
// #define LOGIFS_DEBUG(str)
// #define LOGIFS_WARN(str)
// #define LOGIFS_FATAL(str)

#endif