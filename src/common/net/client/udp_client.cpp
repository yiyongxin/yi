#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "uv.h"

using namespace std;

#define DEFAULT_PORT 8001

uv_loop_t *loop;

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void on_recv(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags)
{
    if (nread < 0) {
        fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) req, NULL);
        free(buf->base);
        return;
    }

    //nread=0表示没有更多的数据可读
    if(nread > 0)
    {
        printf("on_recv:%s,len:%d\r\n", buf->base, nread);
    }
}

void on_send_end(uv_udp_send_t *req, int status)
{
    if (status < 0) {
        fprintf(stderr, "Send error %s\n", uv_strerror(status));
    }

    uv_udp_recv_start(req->handle, alloc_buffer, on_recv);
}



int main() 
{
    loop = uv_default_loop();

    //输入想要发送的数据
    char buffer[100];
    //写入要发生数据的代码

    uv_buf_t buf = uv_buf_init(buffer, sizeof(buffer));
    buf.len = strlen(buffer);
    buf.base = buffer;

    uv_udp_t send_socket;
    uv_udp_init(loop, &send_socket);
    uv_udp_send_t send_req;

    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

    uv_udp_send(&send_req, &send_socket, &buf, 1, (const struct sockaddr *)&addr, on_send_end);

    return uv_run(loop, UV_RUN_DEFAULT);
}