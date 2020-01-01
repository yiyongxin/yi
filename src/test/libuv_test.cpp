#include "../common/net/server/TcpServer.h"
#include "uv.h"
#include <thread>
#include <iostream>

namespace libuv_test
{
    void test();
}

void libuv_test::test()
{
    uv_loop_t* loop = uv_default_loop();
    TcpServer* tcp = new TcpServer(loop,"test server");
    tcp->start("0.0.0.0", 8890);
    std::thread* temp = new std::thread([&](){uv_run(loop, UV_RUN_DEFAULT);});
    char a[100];
    std::cin >> a;
}