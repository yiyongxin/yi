/***************************************
* brief:    基于linuv的tcp client封装
* author:   易雍鑫
* date:     2018-03-27
****************************************/

#include "tcp_client.h"

tcp_client::tcp_client(uv_loop_t* loop)  
    :recvcb_(nullptr),userdata_(nullptr)  
    ,connectstatus_(CONNECT_DIS)  
    , isinit_(false)  
{  
    readbuffer_ = uv_buf_init((char*) malloc(BUFFERSIZE), BUFFERSIZE);  
    writebuffer_ = uv_buf_init((char*) malloc(BUFFERSIZE), BUFFERSIZE);  
    loop_ = loop;  
    connect_req_.data = this;  
    write_req_.data = this;  
}  
  
  
tcp_client::~tcp_client()  
{  
    free(readbuffer_.base);  
    readbuffer_.base = nullptr;  
    readbuffer_.len = 0;  
    free(writebuffer_.base);  
    writebuffer_.base = nullptr;  
    writebuffer_.len = 0;  
    close();  
    LOGIFS_INFO("客户端("<<this<<")退出");  
}  
//初始化与关闭--服务器与客户端一致  
bool tcp_client::init()  
{  
    if (isinit_) {  
        return true;  
    }  
  
    if (!loop_) {  
        errmsg_ = "loop is null on tcp init.";  
        LOGIFS_ERR(errmsg_);  
        return false;  
    }  
    int iret = uv_tcp_init(loop_,&client_);  
    if (iret) {  
        errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(errmsg_);  
        return false;  
    }  
    iret = uv_mutex_init(&write_mutex_handle_);  
    if (iret) {  
        errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(errmsg_);  
        return false;  
    }  
    isinit_ = true;  
    fprintf(stdout,"客户端(%p) init type = %d\n",&client_,client_.type);  
    client_.data = this;  
    //iret = uv_tcp_keepalive(&client_, 1, 60);//  
    //if (iret) {  
    //    errmsg_ = GetUVError(iret);  
    //    return false;  
    //}  
    LOGIFS_INFO("客户端("<<this<<")Init");  
    return true;  
}  
  
void tcp_client::close()  
{  
    if (!isinit_) {  
        return;  
    }  
    uv_mutex_destroy(&write_mutex_handle_);  
    uv_close((uv_handle_t*) &client_, AfterClose);  
    LOGIFS_INFO("客户端("<<this<<")close");  
    isinit_ = false;  
}  
  
bool tcp_client::run(int status)  
{  
    LOGIFS_INFO("客户端("<<this<<")run");  
    int iret = uv_run(loop_,(uv_run_mode)status);  
    if (iret) {  
        errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(errmsg_);  
        return false;  
    }  
    return true;  
}  
  
//属性设置--服务器与客户端一致  
bool tcp_client::setNoDelay(bool enable)  
{  
    //http://blog.csdn.net/u011133100/article/details/21485983  
    int iret = uv_tcp_nodelay(&client_, enable ? 1 : 0);  
    if (iret) {  
        errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(errmsg_);  
        return false;  
    }  
    return true;  
}  
  
bool tcp_client::setKeepAlive(int enable, unsigned int delay)  
{  
    int iret = uv_tcp_keepalive(&client_, enable , delay);  
    if (iret) {  
        errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(errmsg_);  
        return false;  
    }  
    return true;  
}  
  
//作为client的connect函数  
bool tcp_client::connect(const char* ip, int port)  
{  
    close();  
    init();  
    connectip_ = ip;  
    connectport_ = port;  
    LOGIFS_INFO("客户端("<<this<<")start connect to server("<<ip<<":"<<port<<")");  
    int iret = uv_thread_create(&connect_threadhanlde_, ConnectThread, this);//触发AfterConnect才算真正连接成功，所以用线程  
    if (iret) {  
        errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(errmsg_);  
        return false;  
    }  
    while ( connectstatus_ == CONNECT_DIS) {  
#if defined (WIN32) || defined(_WIN32)  
        Sleep(100);  
#else  
        usleep((100) * 1000)  
#endif  
    }  
    return connectstatus_ == CONNECT_FINISH;  
}  
  
bool tcp_client::connect6(const char* ip, int port)  
{  
    close();  
    init();  
    connectip_ = ip;  
    connectport_ = port;  
    LOGIFS_INFO("客户端("<<this<<")start connect to server("<<ip<<":"<<port<<")");  
    int iret = uv_thread_create(&connect_threadhanlde_, ConnectThread6, this);//触发AfterConnect才算真正连接成功，所以用线程  
    if (iret) {  
        errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(errmsg_);  
        return false;  
    }  
    while ( connectstatus_ == CONNECT_DIS) {  
        //fprintf(stdout,"client(%p) wait, connect status %d\n",this,connectstatus_);  
#if defined (WIN32) || defined(_WIN32)  
        Sleep(100);  
#else  
        usleep((100) * 1000)  
#endif  
    }  
    return connectstatus_ == CONNECT_FINISH;  
}  
  
void tcp_client::ConnectThread( void* arg )  
{  
    tcp_client *pclient = (tcp_client*)arg;  
    fprintf(stdout,"client(%p) ConnectThread start\n",pclient);  
    struct sockaddr_in bind_addr;  
    int iret = uv_ip4_addr(pclient->connectip_.c_str(), pclient->connectport_, &bind_addr);  
    if (iret) {  
        pclient->errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(pclient->errmsg_);  
        return;  
    }  
    iret = uv_tcp_connect(&pclient->connect_req_, &pclient->client_, (const sockaddr*)&bind_addr, AfterConnect);  
    if (iret) {  
        pclient->errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(pclient->errmsg_);  
        return;  
    }  
    fprintf(stdout,"client(%p) ConnectThread end, connect status %d\n",pclient, pclient->connectstatus_);  
    pclient->run();  
}  
  
  
void tcp_client::ConnectThread6( void* arg )  
{  
    tcp_client *pclient = (tcp_client*)arg;  
    LOGIFS_INFO("客户端("<<pclient<<")Enter Connect Thread.");  
    fprintf(stdout,"client(%p) ConnectThread start\n",pclient);  
    struct sockaddr_in6 bind_addr;  
    int iret = uv_ip6_addr(pclient->connectip_.c_str(), pclient->connectport_, &bind_addr);  
    if (iret) {  
        pclient->errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(pclient->errmsg_);  
        return;  
    }  
    iret = uv_tcp_connect(&pclient->connect_req_, &pclient->client_, (const sockaddr*)&bind_addr, AfterConnect);  
    if (iret) {  
        pclient->errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(pclient->errmsg_);  
        return;  
    }  
    fprintf(stdout,"client(%p) ConnectThread end, connect status %d\n",pclient, pclient->connectstatus_);  
    LOGIFS_INFO("客户端("<<pclient<<")Leave Connect Thread. connect status "<<pclient->connectstatus_);  
    pclient->run();  
}  
  
void tcp_client::AfterConnect(uv_connect_t* handle, int status)  
{  
    fprintf(stdout,"start after connect\n");  
    tcp_client *pclient = (tcp_client*)handle->handle->data;  
    if (status) {  
        pclient->connectstatus_ = CONNECT_ERROR;  
        fprintf(stdout,"connect error:%s\n",GetUVError(status));  
        return;  
    }  
  
    int iret = uv_read_start(handle->handle, onAllocBuffer, AfterClientRecv);//客户端开始接收服务器的数据  
    if (iret) {  
        fprintf(stdout,"uv_read_start error:%s\n",GetUVError(iret));  
        pclient->connectstatus_ = CONNECT_ERROR;  
    } else {  
        pclient->connectstatus_ = CONNECT_FINISH;  
    }  
    LOGIFS_INFO("客户端("<<pclient<<")run");  
    fprintf(stdout,"end after connect\n");  
}  
  
//客户端的发送函数  
int tcp_client::send(const char* data, std::size_t len)  
{  
    //自己控制data的生命周期直到write结束  
    if (!data || len <= 0) {  
        errmsg_ = "send data is null or len less than zero.";  
        return 0;  
    }  
  
    uv_mutex_lock(&write_mutex_handle_);  
    if (writebuffer_.len < len) {  
        writebuffer_.base = (char*)realloc(writebuffer_.base,len);  
        writebuffer_.len = len;  
    }  
    memcpy(writebuffer_.base,data,len);  
    uv_buf_t buf = uv_buf_init((char*)writebuffer_.base,len);  
    int iret = uv_write(&write_req_, (uv_stream_t*)&client_, &buf, 1, AfterSend);  
    if (iret) {  
        uv_mutex_unlock(&write_mutex_handle_);  
        errmsg_ = GetUVError(iret);  
        LOGIFS_ERR(errmsg_);  
        return false;  
    }  
    return true;  
}  
  
//客户端-接收数据回调函数  
void tcp_client::setrecvcb(client_recvcb cb, void* userdata )  
{  
    recvcb_ = cb;  
    userdata_ = userdata;  
}  
  
//客户端分析空间函数  
void tcp_client::onAllocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)  
{  
    if (!handle->data) {  
        return;  
    }  
    tcp_client *client = (tcp_client*)handle->data;  
    *buf = client->readbuffer_;  
}  
  
  
void tcp_client::AfterClientRecv(uv_stream_t *handle, ssize_t nread, const uv_buf_t* buf)  
{  
    if (!handle->data) {  
        return;  
    }  
    tcp_client *client = (tcp_client*)handle->data;//服务器的recv带的是tcp_client  
    if (nread < 0) {  
        if (nread == UV_EOF) {  
            fprintf(stdout,"服务器(%p)主动断开\n",handle);  
            LOGW("服务器主动断开");  
        } else if (nread == UV_ECONNRESET) {  
            fprintf(stdout,"服务器(%p)异常断开\n",handle);  
            LOGW("服务器异常断开");  
        } else {  
            fprintf(stdout,"服务器(%p)异常断开:%s\n",handle,GetUVError(nread));  
            LOGW("服务器异常断开"<<GetUVError(nread));  
        }  
        uv_close((uv_handle_t*)handle, AfterClose);  
        return;  
    }  
    if (nread > 0 && client->recvcb_) {  
        client->recvcb_(buf->base,nread,client->userdata_);  
    }  
}  
  
//服务器与客户端一致  
void tcp_client::AfterSend(uv_write_t *req, int status)  
{  
    tcp_client *client = (tcp_client *)req->handle->data;  
    uv_mutex_unlock(&client->write_mutex_handle_);  
    if (status < 0) {  
        LOGIFS_ERR("发送数据有误:"<<GetUVError(status));  
        fprintf(stderr, "Write error %s\n", GetUVError(status));  
    }  
}  
//服务器与客户端一致  
void tcp_client::AfterClose(uv_handle_t *handle)  
{  
    fprintf(stdout,"客户端(%p)已close\n",handle);  
    LOGIFS_INFO("客户端("<<handle<<")已close");  
}  
  
void tcp_client::StartLog( const char* logpath /*= nullptr*/ )  
{  
    zsummer::log4z::ILog4zManager::GetInstance()->SetLoggerMonthdir(LOG4Z_MAIN_LOGGER_ID, true);  
    zsummer::log4z::ILog4zManager::GetInstance()->SetLoggerDisplay(LOG4Z_MAIN_LOGGER_ID,false);  
    zsummer::log4z::ILog4zManager::GetInstance()->SetLoggerLevel(LOG4Z_MAIN_LOGGER_ID,LOG_LEVEL_DEBUG);  
    zsummer::log4z::ILog4zManager::GetInstance()->SetLoggerLimitSize(LOG4Z_MAIN_LOGGER_ID,100);  
    if (logpath) {  
        zsummer::log4z::ILog4zManager::GetInstance()->SetLoggerPath(LOG4Z_MAIN_LOGGER_ID,logpath);  
    }  
    zsummer::log4z::ILog4zManager::GetInstance()->Start();  
}  