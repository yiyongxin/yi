#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uv.h"

using namespace std;

uv_loop_t *loop;
struct sockaddr_in req_addr;


void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void echo_read(uv_stream_t *server, ssize_t nread, const uv_buf_t *buf)
{
  if (nread < 0) {
    fprintf(stderr, "error echo_read");
    return;
  }

  printf("response from server: %s\n", buf->base);
}


void on_write_end(uv_write_t *req, int status)
{
  if (status < 0) {
    fprintf(stderr, "error on_write_end");
    return;
  }

  uv_read_start(req->handle, alloc_buffer, echo_read);
}

//连接tcp server
void on_connect(uv_connect_t *req, int status)
{
  if (status < 0) {
    fprintf(stderr, "On_connection error: %d\n", status);
    return;
  }

  //输入想要发送的数据
  char buffer[100];
  //写入要发生数据的代码

  uv_buf_t buf = uv_buf_init(buffer, sizeof(buffer));

  buf.len = strlen(buffer);
  buf.base = buffer;

  uv_stream_t* tcp = req->handle;

  uv_write_t write_req;

  int buf_count = 1;
  uv_write(&write_req, tcp, &buf, buf_count, on_write_end);
}

int main(void) 
{

  loop = uv_default_loop();

  uv_tcp_t client;
  uv_tcp_init(loop, &client);

 //服务器端ip和port
  uv_ip4_addr("0.0.0.0", 8000, &req_addr);

  uv_connect_t connect_req;

  //与server建立连接
  uv_tcp_connect(&connect_req, &client, (const struct sockaddr*)&req_addr, on_connect);

  return uv_run(loop, UV_RUN_DEFAULT);
}