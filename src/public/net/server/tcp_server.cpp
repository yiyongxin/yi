/*************************************** 
* brief:    基于linuv的tcp server封装
* author:   易雍鑫
* date:     2018-03-12
****************************************/ 

#include "stdlib.h"
#include <string.h>
#include "tcp_server.h"
#include "logifs.h"

inline std::string GetUVError(int errcode)  
{  
    std::string err = uv_err_name(errcode);  
    err = err + ": " + uv_strerror(errcode);  
    return std::move(err);  
}  

tcp_server::tcp_server(uv_loop_t* loop)
    :_newconcb(nullptr), _isinit(false),_loop(loop)
{    
     
}   
  
tcp_server::~tcp_server()  
{  
    close();  
    LOGIFS_INFO("tcp server destructor");  
}  

const char* tcp_server::get_errstr()
{
    return _errstr.c_str();
}

bool tcp_server::init()
{  
    if (_isinit)
        return true;  
    if (!_loop) 
    {  
        _errstr = "tcp init loop is null";  
        LOGIFS_ERR(_errstr);  
        return false;  
    }  
    int errcode = uv_tcp_init(_loop, &_server);  
    if (errcode) 
    {  
        _errstr = GetUVError(errcode);  
        LOGIFS_ERR(_errstr);  
        return false;  
    }
    _isinit = true;  
    _server.data = this;  
    //errcode = uv_tcp_keepalive(&_server, 1, 60);//调用此函数后后续函数会调用出错  
    //if (errcode) {  
    //  _errstr = GetUVError(errcode);  
    //  return false;  
    //}  
    return true;  
}  
  
void tcp_server::close()  
{  
    for (auto it = _clienttab.begin(); it!=_clienttab.end(); ++it) 
        uv_close((uv_handle_t*)it->second->_client,AfterClientClose);  
    _clienttab.clear();  
    if (_isinit) 
        uv_close((uv_handle_t*) &_server, AfterServerClose);  
    _isinit = false;  
    LOGIFS_INFO("close server");  
}  
  
bool tcp_server::run(int status)  
{  
    LOGIFS_INFO("server runing.");  
    int iret = uv_run(_loop,(uv_run_mode)status);  
    if (iret) 
    {  
        _errstr = GetUVError(iret);  
        LOGIFS_ERR(_errstr);  
        return false;  
    }  
    return true;  
}  

bool tcp_server::set_nodelay(bool enable) 
{  
    int iret = uv_tcp_nodelay(&_server, enable ? 1 : 0);  
    if (iret) 
    {  
        _errstr = GetUVError(iret);  
        LOGIFS_ERR(_errstr);  
        return false;  
    }  
    return true;  
}  
  
bool tcp_server::set_keep_alive(int enable, unsigned int delay)  
{  
    int iret = uv_tcp_keepalive(&_server, enable , delay);  
    if (iret) 
    {  
        _errstr = GetUVError(iret);  
        LOGIFS_ERR(_errstr);  
        return false;  
    }  
    return true;  
}  
  

bool tcp_server::bind(const char* ip, int port, bool isipv6)  
{  
    if(isipv6)
    {
        struct sockaddr_in6 bind_addr;  
        int iret = uv_ip6_addr(ip, port, &bind_addr);  
        if (iret) 
        {  
            _errstr = GetUVError(iret);  
            LOGIFS_ERR(_errstr);  
            return false;  
        }  
        iret = uv_tcp_bind(&_server, (const struct sockaddr*)&bind_addr,0);  
        if (iret) 
        {  
            _errstr = GetUVError(iret);  
            LOGIFS_ERR(_errstr);  
            return false;  
        }  
    }
    else
    {
        struct sockaddr_in bind_addr;  
        int iret = uv_ip4_addr(ip, port, &bind_addr);  
        if (iret) 
        {  
            _errstr = GetUVError(iret);  
            LOGIFS_ERR(_errstr);  
            return false;  
        }  
        iret = uv_tcp_bind(&_server, (const struct sockaddr*)&bind_addr,0);  
        if (iret) 
        {  
            _errstr = GetUVError(iret);  
            LOGIFS_ERR(_errstr);  
            return false;  
        }  
    }
    LOGIFS_INFO("server bind ip="<<ip<<", port="<<port);  
    return true;  
}  
  
bool tcp_server::listen(int backlog)  
{  
    int iret = uv_listen((uv_stream_t*) &_server, backlog, acceptConnection);  
    if (iret) 
    {  
        _errstr = GetUVError(iret);  
        LOGIFS_ERR(_errstr);  
        return false;  
    }  
    LOGIFS_INFO("server listen");  
    return true;  
}  
  
bool tcp_server::Start( const char *ip, int port )  
{  
    close();  
    if (!init())  
        return false;  
    if (!bind(ip,port))
        return false;  
    if (!listen(SOMAXCONN))
        return false;  
    if (!run()) 
        return false;  
    LOGIFS_INFO("start listen "<<ip<<": "<<port);  
    return true;  
}  

int tcp_server::send(int cid, const char* data, size_t len)  
{  
    auto itfind = _clienttab.find(cid);  
    if (itfind == _clienttab.end()) 
    {  
        _errstr = "can't find cliendid ";  
        _errstr += std::to_string((long long)cid);  
        LOGIFS_ERR(_errstr);  
        return -1;  
    }  
    //自己控制data的生命周期直到write结束  
    if (itfind->second->_writebuf.len < len) 
    {  
        itfind->second->_writebuf.base = (char*)realloc(itfind->second->_writebuf.base,len);  
        itfind->second->_writebuf.len = len;  
    }  
    memcpy(itfind->second->_writebuf.base,data,len);  
    uv_buf_t buf = uv_buf_init((char*)itfind->second->_writebuf.base,len);  
    int iret = uv_write(&itfind->second->_writereq, (uv_stream_t*)itfind->second->_client, &buf, 1, AfterSend);  
    if (iret) 
    {  
        _errstr = GetUVError(iret);  
        LOGIFS_ERR(_errstr);  
        return false;
    }  
    return true;  
}  
  
//服务器-新客户端函数  
void tcp_server::acceptConnection(uv_stream_t *server, int status)  
{  
    if (!server->data) 
        return;  
    tcp_server *tcpsock = (tcp_server *)server->data;  
    int cid = tcpsock->get_cid();  
    tcp_client_obj* cdata = new tcp_client_obj(cid);//uv_close回调函数中释放  
    cdata->_server = tcpsock;//保存服务器的信息  
    int iret = uv_tcp_init(tcpsock->_loop, cdata->_client);//析构函数释放  
    if (iret) 
    {  
        delete cdata;  
        tcpsock->_errstr = GetUVError(iret);  
        LOGIFS_ERR(tcpsock->_errstr);  
        return;  
    }  
    iret = uv_accept((uv_stream_t*)&tcpsock->_server, (uv_stream_t*) cdata->_client);  
    if (iret) 
    {  
        tcpsock->_errstr = GetUVError(iret);  
        uv_close((uv_handle_t*) cdata->_client, NULL);  
        delete cdata;  
        LOGIFS_ERR(tcpsock->_errstr);  
        return;  
    }  
    tcpsock->_clienttab.insert(std::make_pair(cid,cdata));//加入到链接队列  
    if (tcpsock->_newconcb) 
        tcpsock->_newconcb(cid);  
    LOGIFS_INFO("new client("<<cdata->_client<<") id="<< cid);  
    iret = uv_read_start((uv_stream_t*)cdata->_client, onAllocBuffer, AfterServerRecv);//服务器开始接收客户端的数据  
    return;  
}  
  
void tcp_server::setrecvcb(int cid,tcp_client_obj::srecv_cb cb) 
{  
    auto itfind = _clienttab.find(cid);  
    if (itfind != _clienttab.end()) 
        itfind->second->_recvcb = cb;  
}  
  
void tcp_server::setnewcon_cb(newcon_cb cb)  
{  
    _newconcb = cb;  
}  
 
void tcp_server::onAllocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)  
{  
    if (!handle->data) {  
        return;  
    }  
    tcp_client_obj *client = (tcp_client_obj*)handle->data;  
    *buf = client->_readbuf;  
}  
  
void tcp_server::AfterServerRecv(uv_stream_t *handle, ssize_t nread, const uv_buf_t* buf)  
{  
    if (!handle->data) {  
        return;  
    }  
    tcp_client_obj *client = (tcp_client_obj*)handle->data;//服务器的recv带的是tcp_client_obj  
    if (nread < 0) {
        tcp_server *server = (tcp_server *)client->_server;  
        if (nread == UV_EOF) {  
            fprintf(stdout,"客户端(%d)连接断开，关闭此客户端\n",client->id);  
            LOGIFS_WARN("客户端("<<client->id<<")主动断开");  
        } else if (nread == UV_ECONNRESET) {  
            fprintf(stdout,"客户端(%d)异常断开\n",client->id);  
            LOGIFS_WARN("客户端("<<client->id<<")异常断开");  
        } else {  
            fprintf(stdout,"%s\n",GetUVError(nread));  
            LOGIFS_WARN("客户端("<<client->id<<")异常断开："<<GetUVError(nread));  
        }  
        server->delete_client(client->id);//连接断开，关闭客户端  
        return;  
    } else if (0 == nread)  {/* Everything OK, but nothing read. */  
  
    } else if (client->_recvcb) {  
        client->_recvcb(client->id,buf->base,nread);  
    }  
}  
  
void tcp_server::AfterSend(uv_write_t *req, int status)  
{  
    if (status < 0) {  
        LOGIFS_ERR("发送数据有误:"<<GetUVError(status));  
        fprintf(stderr, "Write error %s\n", GetUVError(status));  
    }  
}  
  
void tcp_server::AfterServerClose(uv_handle_t *handle)  
{  
    //服务器,不需要做什么  
}  
  
void tcp_server::AfterClientClose(uv_handle_t *handle)  
{  
    tcp_client_obj *cdata = (tcp_client_obj*)handle->data;  
    LOGIFS_INFO("client "<<cdata->id<<" had closed.");  
    delete cdata;  
}  
  
int tcp_server::get_cid() 
{  
    static int s_id = 0;  
    return ++s_id;  
}  
  
bool tcp_server::delete_client(int cid)  
{  
    // uv_mutex_lock(&mutex_handle_);  
    auto itfind = _clienttab.find(cid);  
    if (itfind == _clienttab.end()) {  
        _errstr = "can't find client ";  
        _errstr += std::to_string((long long)cid);  
        LOGIFS_ERR(_errstr);  
        // uv_mutex_unlock(&mutex_handle_);  
        return false;  
    }  
    if (uv_is_active((uv_handle_t*)itfind->second->_client)) {  
        uv_read_stop((uv_stream_t*)itfind->second->_client);  
    }  
    uv_close((uv_handle_t*)itfind->second->_client,AfterClientClose);  
  
    _clienttab.erase(itfind);  
    LOGIFS_INFO("删除客户端"<<cid);  
    // uv_mutex_unlock(&mutex_handle_);  
    return true;  
} 