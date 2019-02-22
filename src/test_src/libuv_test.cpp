#include "../public/net/server/tcpServer.h"
#include "libuv/uv.h"

namespace libuv_test
{
    void test();
}

void libuv_test::test()
{
    uv_loop_t* loop = uv_default_loop();
    tcpServer* tcp = new tcpServer(loop,"test server");
    tcp->start("0.0.0.0", 8890);
    uv_run(loop, UV_RUN_DEFAULT);
}