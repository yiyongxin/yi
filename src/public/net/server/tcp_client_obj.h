/***************************************
* brief:    tcp server客户端链表存储的客户端类型
* author:   易雍鑫
* date:     2018-03-26
****************************************/

#ifndef TCP_CLIENT_OBJ_H_
#define TCP_CLIENT_OBJ_H_

#include "libuv/uv.h"

#define DEF_BUF_SIZE 1024*1024

class tcpServer;

class tcp_client_obj
{
public:
    typedef void (*srecv_cb)(unsigned int cid, const char* buf, int size);
public:
    tcp_client_obj(unsigned int id);
    ~tcp_client_obj();
public:
    unsigned int id;       //客户端id,惟一
    uv_tcp_t* _client;     //客户端句柄
    tcpServer* _server;   //服务器句柄(保存是因为某些回调函数需要到)
    
    uv_buf_t _readbuf;     //接受数据的buf
    srecv_cb _recvcb;      //接收数据回调给用户的函数
};

#endif //TCP_CLIENT_OBJ_H_