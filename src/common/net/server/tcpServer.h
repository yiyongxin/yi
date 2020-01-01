/***************************************
* brief:    基于linuv的tcp server封装
* author:   易雍鑫
* date:     2018-03-12
****************************************/

#ifndef TcpServer_H_
#define TcpServer_H_

#include <unordered_map>
#include <functional>
#include "uv.h"
#include "TcpClientObj.h"

#define WRITE_POOL_GRAIN_SIZE 1024

using namespace std;

typedef std::function<void(std::string,uint32_t,char*,ssize_t)> srecv_cb;
typedef std::function<void(std::string,uint32_t)> accept_cb;
typedef std::function<void(std::string,uint32_t)> cliClose_cb;

class TcpServer
{
public:
    TcpServer(uv_loop_t* loop,const char* name = "default server");
    ~TcpServer();
private:
    std::string _svr_name;  //该服务的名字
    uv_loop_t *_loop;      //libuv主循环
    uv_tcp_t _server;      //服务器句柄
    bool _isinit;          //是否已初始化
    std::string _errstr;   //错误信息
    std::unordered_map<uint32_t,TcpClientObj*> _clienttab; //客户端哈希表
    uint32_t _sureCid;  //可用客户端id
    accept_cb _acceptcb;        //新连接回调函数
    srecv_cb _recvcb;           //接收数据回调给用户的函数
    cliClose_cb _cliClosecb;    //客户端关闭回调
public:
    bool start(const char *ip, int port, int backlog = 1024, bool isipv6 = false);
    void close();

    bool setnodelay(bool enable);  //设置是否开启小包合并发送功能
    bool setKeepAlive(int enable, uint32_t delay);    //设置心跳函数

    const char* getErrStr();   //获取错误字符串
private:
    bool init();  //初始化
    bool run(int status = UV_RUN_DEFAULT);
    bool bind(const char* ip, int port, bool isipv6 = false);   //绑定ip和端口
    bool listen(int backlog);        //开始监听
    uint32_t getCid();                 //获取可用客户端编号
    bool deleteClient(uint32_t cid);   //删除客户端
private:
private:
    static void severClose_cb(uv_handle_t *handle);    //关闭服务端后回调函数
    static void clientClose_cb(uv_handle_t *handle);   //关闭客户端后回调函数
    static void send_cb(uv_write_t *req, int status);   //发送后回调函数
    static void accept(uv_stream_t *handle, int status);    //接受连接
    static void readAlloc_cb(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t* buf);
public:
    bool send(uint32_t cid, const char* data, size_t len);
    void setaccept_cb(accept_cb cb);  //服务器-新链接回调函数
    void setrecv_cb(srecv_cb cb);//设置接收回调函数，每个客户端各有一个
};

#endif //TcpServer_H_