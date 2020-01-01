/***************************************
* brief:    tcp server客户端链表存储的客户端类型
* author:   易雍鑫
* date:     2018-03-26
****************************************/

#ifndef TcpClientObj_H_
#define TcpClientObj_H_

#include "uv.h"

#define DEF_BUF_SIZE 1024*1024

class TcpServer;

class TcpClientObj
{
public:
public:
    TcpClientObj(uint32_t id);
    ~TcpClientObj();
public:
    uint32_t id;       //客户端id,惟一
    uv_tcp_t* _client;     //客户端句柄
    TcpServer* _server;   //服务器句柄(保存是因为某些回调函数需要到)
    
    uv_buf_t _readbuf;     //接受数据的buf
};

#endif //TcpClientObj_H_