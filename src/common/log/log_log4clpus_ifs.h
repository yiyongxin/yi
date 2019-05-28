/***************************************
* brief:    log4clpus转接框架宏
* author:   易雍鑫
* date:     2019-02-22
****************************************/
#ifndef LOG_LOG4CLPUS_IFS_H_
#define LOG_LOG4CLPUS_IFS_H_

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
        /*LOG4CPLUS_ERROR(_logger_err,__FILE__ << ":" << __LINE__ << " " << __FUNCTION__);*/\
        LOG4CPLUS_ERROR(_logger_err,str);\
    }

#define LOGIFS_INFO(str)\
    {\
        /*LOG4CPLUS_INFO(_logger_info,__FILE__ << ":" << __LINE__ << " " << __FUNCTION__);*/\
        LOG4CPLUS_INFO(_logger_info,str);\
    }

#define LOGIFS_DEBUG(str)\
    {\
        /*LOG4CPLUS_DEBUG(_logger_debug,__FILE__ << ":" << __LINE__ << " " << __FUNCTION__);*/\
        LOG4CPLUS_DEBUG(_logger_debug,str);\
    }

#define LOGIFS_WARN(str)\
    {\
        /*LOG4CPLUS_WARN(_logger_warn,__FILE__ << ":" << __LINE__ << " " << __FUNCTION__);*/\
        LOG4CPLUS_WARN(_logger_warn,str);\
    }

#define LOGIFS_FATAL(str)\
    {\
        /*LOG4CPLUS_FATAL(_logger_fatal,__FILE__ << ":" << __LINE__ << " " << __FUNCTION__);*/\
        LOG4CPLUS_FATAL(_logger_fatal,str);\
    }

#endif //LOG_LOG4CLPUS_IFS_H_
