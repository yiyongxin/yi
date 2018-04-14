/***************************************
* brief:    tcp server客户端链表存储的客户端类型
* author:   易雍鑫
* date:     2018-03-26
****************************************/

#include "tcpClientObj.h"
#include "stdlib.h"

tcpClientObj::tcpClientObj(unsigned int id)
    :id(id),_recvcb(nullptr)
{
    _client = (uv_tcp_t*)malloc(sizeof(*_client));
    _client->data = this;
    _readbuf = uv_buf_init((char*)malloc(DEF_BUF_SIZE), DEF_BUF_SIZE);
}

tcpClientObj::~tcpClientObj()
{
    free(_readbuf.base);
    _readbuf.base = nullptr;
    _readbuf.len = 0;

    free(_client);
    _client = nullptr;
}