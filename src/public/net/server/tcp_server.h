/***************************************
* brief:    基于linuv的tcp server封装
* author:   易雍鑫
* date:     2018-03-12
****************************************/

#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include <unordered_map>
#include "libuv/uv.h"
#include "tcp_client_obj.h"

class tcp_server
{
public:
    typedef void (*newcon_cb)(int cid);
public:
    tcp_server(uv_loop_t* loop);
    virtual ~tcp_server();
private:
    uv_loop_t *_loop;      //libuv主循环
    uv_tcp_t _server;      //服务器句柄
    bool _isinit;          //是否已初始化
    std::string _errstr;   //错误信息
    std::unordered_map<int,tcp_client_obj*> _clienttab; //客户端哈希表
    newcon_cb _newconcb;   //新连接回调函数
public:
    bool Start(const char *ip, int port);
    void close();

    bool set_nodelay(bool enable);
    bool set_keep_alive(int enable, unsigned int delay);    //设置心跳函数

    const char* get_errstr();   //获取错误字符串
private:
    bool init();  //初始化
    bool run(int status = UV_RUN_DEFAULT);
    bool bind(const char* ip, int port, bool isipv6 = false);   //绑定ip和端口
    bool listen(int backlog = 1024);    //开始监听
    int get_cid();  //获取可用客户端编号
    bool delete_client(int cid);   //删除客户端
private:
    static void AfterServerRecv(uv_stream_t *client, ssize_t nread, const uv_buf_t* buf);
    static void AfterSend(uv_write_t *req, int status);
    static void onAllocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void AfterServerClose(uv_handle_t *handle);
    static void AfterClientClose(uv_handle_t *handle);
    static void acceptConnection(uv_stream_t *server, int status);
public:
    virtual int  send(int cid, const char* data, size_t len);
    virtual void setnewcon_cb(newcon_cb cb);  //服务器-新链接回调函数
    virtual void setrecvcb(int cid,tcp_client_obj::srecv_cb cb);//设置接收回调函数，每个客户端各有一个
};

#endif //TCP_SERVER_H_