/***************************************
* brief:    入口函数
* author:   易雍鑫
* date:     2018-04-08
****************************************/

#include "public/net/server/tcp_server.h"
#include "libuv/uv.h"
#include <stdlib.h>  
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("test libuv net\n"); 
    uv_loop_t* loop = uv_default_loop();
    tcp_server* tcp = new tcp_server(loop,"test server");
    tcp->start("0.0.0.0", 8890);
    uv_run(loop, UV_RUN_DEFAULT);
    return 1;
}