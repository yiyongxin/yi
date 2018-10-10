/***************************************
* brief:    基于linuv的tcp server封装
* author:   易雍鑫
* date:     2018-03-12
****************************************/

#ifndef tcpServer_H_
#define tcpServer_H_

#include <boost/pool/pool.hpp>
#include <unordered_map>
#include "libuv/uv.h"
#include "tcpClientObj.h"

#define WRITE_POOL_GRAIN_SIZE 1024

class tcpServer
{
public:
    typedef void (*newcon_cb)(int cid);
public:
    tcpServer(uv_loop_t* loop,char* name = "default server");
    ~tcpServer();
private:
    std::string _svr_name;  //该服务的名字
    uv_loop_t *_loop;      //libuv主循环
    uv_tcp_t _server;      //服务器句柄
    bool _isinit;          //是否已初始化
    std::string _errstr;   //错误信息
    std::unordered_map<unsigned int,tcpClientObj*> _clienttab; //客户端哈希表
    newcon_cb _newconcb;    //新连接回调函数
    unsigned int _sureCid;  //可用客户端id
public:
    bool start(const char *ip, int port, int backlog = 1024, bool isipv6 = false);
    void close();

    bool setnodelay(bool enable);  //设置是否开启小包合并发送功能
    bool setKeepAlive(int enable, unsigned int delay);    //设置心跳函数

    const char* getErrStr();   //获取错误字符串
private:
    bool init();  //初始化
    bool run(int status = UV_RUN_DEFAULT);
    bool bind(const char* ip, int port, bool isipv6 = false);   //绑定ip和端口
    bool listen(int backlog);        //开始监听
    unsigned int getCid();                 //获取可用客户端编号
    bool deleteClient(unsigned int cid);   //删除客户端
private:
    static boost::pool<> _reqpool;
    // static gspool _writ_buf_pool;
private:
    static void severClose_cb(uv_handle_t *handle);    //关闭服务端后回调函数
    static void clientClose_cb(uv_handle_t *handle);   //关闭客户端后回调函数
    static void send_cb(uv_write_t *req, int status);   //发送后回调函数
    static void accept(uv_stream_t *handle, int status);    //接受连接
    static void readAlloc_cb(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t* buf);
public:
    bool send(unsigned int cid, const char* data, size_t len);
    void setnewcon_cb(newcon_cb cb);  //服务器-新链接回调函数
    void setrecv_cb(unsigned int cid,tcpClientObj::srecv_cb cb);//设置接收回调函数，每个客户端各有一个
};

#endif //tcpServer_H_