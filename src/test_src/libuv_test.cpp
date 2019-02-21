#include "../public/net/server/tcpServer.h"
#include "libuv/uv.h"

// #include <log4cplus/logger.h>
// #include <log4cplus/configurator.h>
// #include <log4cplus/fileappender.h>
// #include <log4cplus/consoleappender.h>
// #include <log4cplus/layout.h>
// #include <log4cplus/loggingmacros.h> 

// log4cplus::Logger _logger_err;
// log4cplus::Logger _logger_debug;
// log4cplus::Logger _logger_info;
// log4cplus::Logger _logger_warn;
// log4cplus::Logger _logger_fatal;

namespace libuv_test
{
    void test();
}

void libuv_test::test()
{
    // log4cplus::SharedAppenderPtr _append_err(new log4cplus::FileAppender("err.log"));
    // _append_err->setName("filelogtest");
    // _logger_err = log4cplus::Logger::getInstance("test.subtestof_filelog");
    // _logger_err.addAppender(_append_err);
    
    // log4cplus::SharedAppenderPtr _append_debug(new log4cplus::FileAppender("debug.log"));
    // _append_debug->setName("filelogtest");
    // _logger_debug = log4cplus::Logger::getInstance("test.subtestof_filelog");
    // _logger_debug.addAppender(_append_debug);

    // log4cplus::SharedAppenderPtr _append_info(new log4cplus::FileAppender("info.log"));
    // _append_info->setName("filelogtest");
    // _logger_info = log4cplus::Logger::getInstance("test.subtestof_filelog");
    // _logger_info.addAppender(_append_info);

    // log4cplus::SharedAppenderPtr _append_warn(new log4cplus::FileAppender("warn.log"));
    // _append_warn->setName("filelogtest");
    // _logger_warn = log4cplus::Logger::getInstance("test.subtestof_filelog");
    // _logger_warn.addAppender(_append_warn);

    // log4cplus::SharedAppenderPtr _append_fatal(new log4cplus::FileAppender("fatal.log"));
    // _append_fatal->setName("filelogtest");
    // _logger_fatal = log4cplus::Logger::getInstance("test.subtestof_filelog");
    // _logger_fatal.addAppender(_append_fatal);

    uv_loop_t* loop = uv_default_loop();
    tcpServer* tcp = new tcpServer(loop,"test server");
    tcp->start("0.0.0.0", 8890);
    uv_run(loop, UV_RUN_DEFAULT);
}