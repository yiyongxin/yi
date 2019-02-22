#ifdef _LOG4CPLUS
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h> 

log4cplus::Logger _logger_err;
log4cplus::Logger _logger_debug;
log4cplus::Logger _logger_info;
log4cplus::Logger _logger_warn;
log4cplus::Logger _logger_fatal;

void initLog()
{
    log4cplus::initialize();
    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("config/log4cplus.properties"));
    _logger_err = log4cplus::Logger::getInstance("err");
    _logger_debug = log4cplus::Logger::getInstance("debug");
    _logger_info = log4cplus::Logger::getInstance("info");
    _logger_warn = log4cplus::Logger::getInstance("warn");
    _logger_fatal = log4cplus::Logger::getInstance("fatal");
}

#endif
