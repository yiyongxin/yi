/***************************************
* brief:    tcp server客户端链表存储的客户端类型
* author:   易雍鑫
* date:     2018-03-26
****************************************/

#include "tcp_client_obj.h"
#include "stdlib.h"

tcp_client_obj::tcp_client_obj(int id)
    :id(id),_recvcb(nullptr)
{
    _client = (uv_tcp_t*)malloc(sizeof(*_client));
    _client->data = this;
    _readbuf = uv_buf_init((char*)malloc(DEF_BUF_SIZE), DEF_BUF_SIZE);
    _writebuf = uv_buf_init((char*)malloc(DEF_BUF_SIZE), DEF_BUF_SIZE);
}

tcp_client_obj::~tcp_client_obj()
{
    free(_readbuf.base);
    _readbuf.base = nullptr;
    _readbuf.len = 0;

    free(_writebuf.base);
    _writebuf.base = nullptr;
    _writebuf.len = 0;

    free(_client);
    _client = nullptr;
}