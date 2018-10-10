/***************************************
* brief:    基于linuv的tcp server封装
* author:   易雍鑫
* date:     2018-03-12
****************************************/

#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include "tcpServer.h"
#include "../../logifs.h"

inline const char* libuv_err_str(int errcode)
{
    std::string err = uv_err_name(errcode);
    err = err + ": " + uv_strerror(errcode);
    return err.c_str();
}

boost::pool<> tcpServer::_reqpool(sizeof(uv_write_t));

tcpServer::tcpServer(uv_loop_t* loop, char* name)
    :_newconcb(nullptr), _isinit(false),_loop(loop),_svr_name(name)
{

}

tcpServer::~tcpServer()
{
    close();
    LOGIFS_INFO(_svr_name.c_str() << ": tcp server destructor");
}

bool tcpServer::init()
{
    if (_isinit)
        return true;
    if (!_loop)
    {
        _errstr = "tcp init loop is null";
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    int errcode = uv_tcp_init(_loop, &_server);
    if (errcode)
    {
        _errstr = libuv_err_str(errcode);
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    _isinit = true;
    _server.data = this;
    return true;
}

void tcpServer::close()
{
    for (auto it = _clienttab.begin(); it!=_clienttab.end(); ++it)
        uv_close((uv_handle_t*)it->second->_client,clientClose_cb);
    _clienttab.clear();
    if (_isinit)
        uv_close((uv_handle_t*) &_server, severClose_cb);
    _isinit = false;
    LOGIFS_INFO("close server");
}

void tcpServer::severClose_cb(uv_handle_t *handle)
{

}

void tcpServer::clientClose_cb(uv_handle_t *handle)
{
    tcpClientObj *cdata = (tcpClientObj*)handle->data;
    LOGIFS_INFO("client "<<cdata->id<<" had closed.");
    delete cdata;
}

const char* tcpServer::getErrStr()
{
    return _errstr.c_str();
}

// bool tcpServer::run(int status)
// {
//     LOGIFS_INFO("server runing.");
//     int iret = uv_run(_loop,(uv_run_mode)status);
//     if (iret)
//     {
//         _errstr = libuv_err_str(iret);
//         LOGIFS_ERR(_errstr.c_str());
//         return false;
//     }
//     return true;
// }

bool tcpServer::setnodelay(bool enable)
{
    int iret = uv_tcp_nodelay(&_server, enable ? 1 : 0);
    if (iret)
    {
        _errstr = libuv_err_str(iret);
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    return true;
}

bool tcpServer::setKeepAlive(int enable, unsigned int delay)
{
    int iret = uv_tcp_keepalive(&_server, enable , delay);
    if (iret)
    {
        _errstr = libuv_err_str(iret);
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    return true;
}

bool tcpServer::bind(const char* ip, int port, bool isipv6)
{
    if(isipv6)
    {
        struct sockaddr_in6 bind_addr;
        int iret = uv_ip6_addr(ip, port, &bind_addr);
        if (iret)
        {
            _errstr = libuv_err_str(iret);
            LOGIFS_ERR(_errstr.c_str());
            return false;
        }
        iret = uv_tcp_bind(&_server, (const struct sockaddr*)&bind_addr,0);
        if (iret)
        {
            _errstr = libuv_err_str(iret);
            LOGIFS_ERR(_errstr.c_str());
            return false;
        }
    }
    else
    {
        struct sockaddr_in bind_addr;
        int iret = uv_ip4_addr(ip, port, &bind_addr);
        if (iret)
        {
            _errstr = libuv_err_str(iret);
            LOGIFS_ERR(_errstr.c_str());
            return false;
        }
        iret = uv_tcp_bind(&_server, (const struct sockaddr*)&bind_addr,0);
        if (iret)
        {
            _errstr = libuv_err_str(iret);
            LOGIFS_ERR(_errstr.c_str());
            return false;
        }
    }
    LOGIFS_INFO("server bind ip="<<ip<<", port="<<port);
    return true;
}

bool tcpServer::listen(int backlog)
{
    int iret = uv_listen((uv_stream_t*) &_server, backlog, accept);
    if (iret)
    {
        _errstr = libuv_err_str(iret);
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    LOGIFS_INFO("server listen");
    return true;
}

void tcpServer::accept(uv_stream_t *handle, int status)
{
    if (!handle->data)
        return;
    tcpServer *server = (tcpServer *)handle->data;
    unsigned int cid = server->getCid();
    printf("accept cid:%d\n",cid); 
    tcpClientObj* cdata = new tcpClientObj(cid);
    cdata->_server = server;    //保存服务器的信息
    int iret = uv_tcp_init(server->_loop, cdata->_client);
    if (iret)
    {
        delete cdata;
        server->_errstr = libuv_err_str(iret);
        LOGIFS_ERR(server->_errstr.c_str());
        return;
    }
    iret = uv_accept((uv_stream_t*)&server->_server, (uv_stream_t*) cdata->_client);
    if (iret)
    {
        server->_errstr = libuv_err_str(iret);
        uv_close((uv_handle_t*) cdata->_client, NULL);
        delete cdata;
        LOGIFS_ERR(server->_errstr.c_str());
        return;
    }
    iret = uv_read_start((uv_stream_t*)cdata->_client, readAlloc_cb, read_cb); //服务器开始接收客户端的数据
    if (iret)
    {
        server->_errstr = libuv_err_str(iret);
        uv_close((uv_handle_t*) cdata->_client, NULL);
        delete cdata;
        LOGIFS_ERR(server->_errstr.c_str());
        return;
    }
    if (server->_newconcb != NULL)
        server->_newconcb(cid);
    server->_clienttab.insert(std::make_pair(cid,cdata));   //加入到链接队列
    LOGIFS_INFO("new client("<<cdata->_client<<") id="<< cid);
    return;
}

unsigned int tcpServer::getCid()
{
    return ++_sureCid;
}

void tcpServer::readAlloc_cb(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    if (!handle->data) 
        return;
    tcpClientObj *client = (tcpClientObj*)handle->data;
    printf("alloc_cb cid:%d\n",client->id); 
    *buf = client->_readbuf;
}

void tcpServer::read_cb(uv_stream_t *handle, ssize_t nread, const uv_buf_t* buf)
{
    if (!handle->data) 
        return;
    tcpClientObj *client = (tcpClientObj*)handle->data; //服务器的recv带的是tcpClientObj

    printf("read_cb:%s %x cid:%d\n",buf->base,buf->base,client->id); 
    if(nread>0)
    {
        tcpServer *tsvr = client->_server;
        void* tmall = malloc(nread);
        memcpy(tmall,buf->base,nread);
        tsvr->send(client->id,buf->base,nread);
    }

    if (nread < 0) 
    {
        if (nread == UV_EOF) 
        {
            LOGIFS_WARN("客户端("<<client->id<<")主动断开");
        }
        else if (nread == UV_ECONNRESET) 
        {
            LOGIFS_WARN("客户端("<<client->id<<")异常断开");
        } 
        else 
        {
            LOGIFS_WARN("客户端("<<client->id<<")异常断开："<<libuv_err_str(nread));
        }
        tcpServer *server = (tcpServer *)client->_server;
        server->deleteClient(client->id);//连接断开，关闭客户端
        return;
    } else if (0 == nread)  {/* Everything OK, but nothing read. */

    } else if (client->_recvcb) 
        client->_recvcb(client->id,buf->base,nread);
}

bool tcpServer::start(const char *ip, int port, bool isipv6)
{
    close();
    if (!init())
        return false;
    if (!bind(ip,port,isipv6))
        return false;
    if (!listen(SOMAXCONN))
        return false;
    LOGIFS_INFO("start listen "<<ip<<": "<<port);
    return true;
}

bool tcpServer::send(unsigned int cid, const char* data, size_t len)
{
    auto itfind = _clienttab.find(cid);
    if (itfind == _clienttab.end())
    {
        _errstr = "can't find cliendid ";
        _errstr += std::to_string((long long)cid);
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    tcpClientObj* ttco = itfind->second;
    uv_buf_t buf = uv_buf_init((char*)data,len);
    int iret = uv_write((uv_write_t *)_reqpool.malloc(), (uv_stream_t*)ttco->_client, &buf, 1, send_cb);
    if (iret)
    {
        _errstr = libuv_err_str(iret);
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    return true;
}

void tcpServer::send_cb(uv_write_t *req, int status)
{
    if (status < 0)
        LOGIFS_ERR("发送数据有误:"<<libuv_err_str(status));
    uv_buf_t *buf = req->bufs;
    _reqpool.free(req);
}

void tcpServer::setrecv_cb(unsigned int cid,tcpClientObj::srecv_cb cb)
{
    auto itfind = _clienttab.find(cid);
    if (itfind != _clienttab.end())
        itfind->second->_recvcb = cb;
}

void tcpServer::setnewcon_cb(newcon_cb cb)
{
    _newconcb = cb;
}

bool tcpServer::deleteClient(unsigned int cid)
{
    // uv_mutex_lock(&mutex_handle_);
    auto itfind = _clienttab.find(cid);
    if (itfind == _clienttab.end()) 
    {
        _errstr = "can't find client ";
        _errstr += std::to_string((long long)cid);
        LOGIFS_ERR(_errstr.c_str());
        // uv_mutex_unlock(&mutex_handle_);
        return false;
    }
    if (uv_is_active((uv_handle_t*)itfind->second->_client)) 
        uv_read_stop((uv_stream_t*)itfind->second->_client);
    uv_close((uv_handle_t*)itfind->second->_client,clientClose_cb);

    _clienttab.erase(itfind);
    LOGIFS_INFO("删除客户端"<<cid);
    // uv_mutex_unlock(&mutex_handle_);
    return true;
}
