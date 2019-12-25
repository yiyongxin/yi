/***************************************
* brief:    基于linuv的tcp server封装
* author:   易雍鑫
* date:     2018-03-12
****************************************/

#include <string.h>
#include "TcpServer.h"
// #include <jemalloc/jemalloc.h>
#include "../../log/log_ifs.h"

inline void libuv_err_str(std::string &err, int errcode)
{
    err = uv_err_name(errcode);
    err = err + ": " + uv_strerror(errcode);
}

TcpServer::TcpServer(uv_loop_t* loop, const char* name)
    :_svr_name(name),_loop(loop), _isinit(false),_acceptcb(nullptr)
{

}

TcpServer::~TcpServer()
{
    close();
    LOGIFS_INFO(_svr_name.c_str() << ": tcp server destructor");
}

bool TcpServer::init()
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
        libuv_err_str(_errstr,errcode);
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    _isinit = true;
    _server.data = this;
    return true;
}

void TcpServer::close()
{
    for (auto it = _clienttab.begin(); it!=_clienttab.end(); ++it)
        uv_close((uv_handle_t*)it->second->_client,clientClose_cb);
    _clienttab.clear();
    if (_isinit)
        uv_close((uv_handle_t*) &_server, severClose_cb);
    _isinit = false;
    LOGIFS_INFO("close server");
}

void TcpServer::severClose_cb(uv_handle_t *cliHandle)
{

}

void TcpServer::clientClose_cb(uv_handle_t *svrHandle)
{
    TcpClientObj *cdata = (TcpClientObj*)svrHandle->data;
    LOGIFS_INFO("client "<<cdata->id<<" had closed.");
    delete cdata;
}

const char* TcpServer::getErrStr()
{
    return _errstr.c_str();
}

// bool TcpServer::run(int status)
// {
//     LOGIFS_INFO("server runing.");
//     int errcode = uv_run(_loop,(uv_run_mode)status);
//     if (errcode)
//     {
//         libuv_err_str(_errstr,errcode);
//         LOGIFS_ERR(_errstr.c_str());
//         return false;
//     }
//     return true;
// }

bool TcpServer::setnodelay(bool enable)
{
    int errcode = uv_tcp_nodelay(&_server, enable ? 1 : 0);
    if (errcode)
    {
        libuv_err_str(_errstr,errcode);
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    return true;
}

bool TcpServer::setKeepAlive(int enable, uint32_t delay)
{
    int errcode = uv_tcp_keepalive(&_server, enable , delay);
    if (errcode)
    {
        libuv_err_str(_errstr,errcode);
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    return true;
}

bool TcpServer::bind(const char* ip, int port, bool isipv6)
{
    if(isipv6)
    {
        struct sockaddr_in6 bind_addr;
        int errcode = uv_ip6_addr(ip, port, &bind_addr);
        if (errcode)
        {
            libuv_err_str(_errstr,errcode);
            LOGIFS_ERR(_errstr.c_str());
            return false;
        }
        errcode = uv_tcp_bind(&_server, (const struct sockaddr*)&bind_addr,0);
        if (errcode)
        {
            libuv_err_str(_errstr,errcode);
            LOGIFS_ERR(_errstr.c_str());
            return false;
        }
    }
    else
    {
        struct sockaddr_in bind_addr;
        int errcode = uv_ip4_addr(ip, port, &bind_addr);
        if (errcode)
        {
            libuv_err_str(_errstr,errcode);
            LOGIFS_ERR(_errstr.c_str());
            return false;
        }
        errcode = uv_tcp_bind(&_server, (const struct sockaddr*)&bind_addr,0);
        if (errcode)
        {
            libuv_err_str(_errstr,errcode);
            LOGIFS_ERR(_errstr.c_str());
            return false;
        }
    }
    LOGIFS_INFO("server bind ip="<<ip<<", port="<<port);
    return true;
}

bool TcpServer::listen(int backlog)
{
    int errcode = uv_listen((uv_stream_t*) &_server, backlog, accept);
    if (errcode)
    {
        libuv_err_str(_errstr,errcode);
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    LOGIFS_INFO("server listen");
    return true;
}

void TcpServer::accept(uv_stream_t *cliHandle, int status)
{
    if (!cliHandle->data)
        return;
    TcpServer *server = (TcpServer *)cliHandle->data;
    uint32_t cid = server->getCid();
    LOGIFS_DEBUG("accept cid:" << cid); 
    TcpClientObj* cdata = new TcpClientObj(cid);
    cdata->_server = server;    //保存服务器的信息
    int errcode = uv_tcp_init(server->_loop, cdata->_client);
    if (errcode)
    {
        delete cdata;
        libuv_err_str(server->_errstr,errcode);
        LOGIFS_ERR(server->_errstr.c_str());
        return;
    }
    errcode = uv_accept((uv_stream_t*)&server->_server, (uv_stream_t*) cdata->_client);
    if (errcode)
    {
        libuv_err_str(server->_errstr,errcode);
        uv_close((uv_handle_t*) cdata->_client, NULL);
        delete cdata;
        LOGIFS_ERR(server->_errstr.c_str());
        return;
    }
    errcode = uv_read_start((uv_stream_t*)cdata->_client, readAlloc_cb, read_cb); //服务器开始接收客户端的数据
    if (errcode)
    {
        libuv_err_str(server->_errstr,errcode);
        uv_close((uv_handle_t*) cdata->_client, NULL);
        delete cdata;
        LOGIFS_ERR(server->_errstr.c_str());
        return;
    }
    if (server->_acceptcb != NULL)
        server->_acceptcb(server->_svr_name,cid);
    server->_clienttab.insert(std::make_pair(cid,cdata));   //加入到链接队列
    LOGIFS_INFO("new client("<<cdata->_client<<") id="<< cid);
    return;
}

uint32_t TcpServer::getCid()
{
    while(true)
    {
        _sureCid++;
        auto itfind = _clienttab.find(_sureCid);
        if (itfind == _clienttab.end())
            break;
    }
    return _sureCid;
}

void TcpServer::readAlloc_cb(uv_handle_t *svrHandle, size_t suggested_size, uv_buf_t *buf)
{
    if (!svrHandle->data) 
        return;
    TcpClientObj *client = (TcpClientObj*)svrHandle->data;
    LOGIFS_DEBUG("alloc_cb cid:" << client->id); 
    *buf = client->_readbuf;
}

void TcpServer::read_cb(uv_stream_t *svrHandle, ssize_t nread, const uv_buf_t* buf)
{
    if (!svrHandle->data) 
        return;
    TcpClientObj *client = (TcpClientObj*)svrHandle->data; //服务器的recv带的是TcpClientObj

    LOGIFS_DEBUG("read_cb:" << buf->base << " cid:" << client->id); 
    if(nread>0)
    {
        TcpServer *tsvr = client->_server;
        void* tmall = malloc(nread);
        memcpy(tmall,buf->base,nread);
        tsvr->send(client->id,buf->base,nread);
    }

    TcpServer *server = (TcpServer *)client->_server;
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
            libuv_err_str(server->_errstr,nread);
            LOGIFS_WARN("客户端("<<client->id<<")异常断开："<<server->_errstr.c_str());
        }
        server->deleteClient(client->id);//连接断开，关闭客户端
        return;
    } else if (0 == nread)  {/* Everything OK, but nothing read. */

    } else if (server->_recvcb) 
        server->_recvcb(server->_svr_name,client->id,buf->base,nread);
}

bool TcpServer::start(const char *ip, int port, int backlog, bool isipv6)
{
    if (!init())
        return false;
    if (!bind(ip,port,isipv6))
        return false;
    if (!listen(backlog))
        return false;
    LOGIFS_INFO("start listen "<<ip<<": "<<port);
    return true;
}

bool TcpServer::send(uint32_t cid, const char* data, size_t len)
{
    auto itfind = _clienttab.find(cid);
    if (itfind == _clienttab.end())
    {
        _errstr = "can't find cliend id: ";
        _errstr += cid;
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    TcpClientObj* ttco = itfind->second;
    uv_buf_t buf = uv_buf_init((char*)data,len);
    int errcode = uv_write(new uv_write_t(), (uv_stream_t*)ttco->_client, &buf, 1, send_cb);
    if (errcode)
    {
        libuv_err_str(_errstr, errcode);
        LOGIFS_ERR(_errstr.c_str());
        return false;
    }
    return true;
}

void TcpServer::send_cb(uv_write_t *req, int status)
{
    if (status < 0)
    {
        TcpServer* tempServer = (TcpServer*)((TcpClientObj*)(req->handle->data))->_server;
        libuv_err_str(tempServer->_errstr,status);
        LOGIFS_ERR("发送数据有误:"<<tempServer->_errstr.c_str());
    }
    delete req;
}

void TcpServer::setrecv_cb(srecv_cb cb)
{
    _recvcb = cb;
}

void TcpServer::setaccept_cb(accept_cb cb)
{
    _acceptcb = cb;
}

bool TcpServer::deleteClient(uint32_t cid)
{
    // uv_mutex_lock(&mutex_handle_);
    auto itfind = _clienttab.find(cid);
    if (itfind == _clienttab.end()) 
    {
        _errstr = "can't find client id: ";
        _errstr += cid;
        LOGIFS_ERR(_errstr.c_str());
        // uv_mutex_unlock(&mutex_handle_);
        return false;
    }
    if (uv_is_active((uv_handle_t*)itfind->second->_client)) 
        uv_read_stop((uv_stream_t*)itfind->second->_client);
    uv_close((uv_handle_t*)itfind->second->_client,clientClose_cb);

    _clienttab.erase(itfind);
    LOGIFS_INFO("delete client cid:"<<cid);
    // uv_mutex_unlock(&mutex_handle_);
    return true;
}
