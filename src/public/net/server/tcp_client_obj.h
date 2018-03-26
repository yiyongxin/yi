#ifndef TCP_CLIENT_OBJ_H_
#define TCP_CLIENT_OBJ_H_

#include "libuv/uv.h"

#define DEF_BUF_SIZE 1024*1024

class tcp_server;

class tcp_client_obj
{  
public:
    typedef void (*srecv_cb)(int cid, const char* buf, int size);  
public:  
    tcp_client_obj(int id);
    ~tcp_client_obj();
public:
    int id;                 //客户端id,惟一  
    uv_tcp_t* _client;     //客户端句柄  
    tcp_server* _server;   //服务器句柄(保存是因为某些回调函数需要到)  
    uv_buf_t _readbuf;     //接受数据的buf  
    uv_buf_t _writebuf;    //写数据的buf  
    uv_write_t _writereq;  
    srecv_cb _recvcb;      //接收数据回调给用户的函数  
};  

#endif //TCP_CLIENT_OBJ_H_