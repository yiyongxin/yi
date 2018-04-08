/***************************************
* brief:    入口函数
* author:   易雍鑫
* date:     2018-04-08
****************************************/

#include "public/net/server/tcp_server.h"
#include "libuv/uv.h"
#include <iostream>

int main(int argc, char *argv[])
{
    uv_loop_t* loop = uv_default_loop();
    tcp_server* tcp = new tcp_server(loop,"test server");
    tcp->start("127.0.0.1", 8890);
    std::cout << "+++++++";
    uv_run(loop, UV_RUN_DEFAULT);
    std::cout << "?????";
    return 1;
}