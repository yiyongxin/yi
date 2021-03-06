// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// extern "C"
// {
// #include "uv.h"
// }

// #define DEFAULT_PORT 8001

// uv_loop_t *loop;

// void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
// {
//     buf->base = (char*) malloc(suggested_size);
//     buf->len = suggested_size;
// }

// void on_send(uv_udp_send_t *req, int status)
// {
//     if (status < 0) {
//         fprintf(stderr, "Send error %s\n", uv_strerror(status));
//     }
// }

// void on_recv(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags)
// {
//     if (nread < 0) {
//         fprintf(stderr, "Read error %s\n", uv_err_name(nread));
//         uv_close((uv_handle_t*) req, NULL);
//         free(buf->base);
//         return;
//     }

//     char sender[17] = { 0 };
//     uv_ip4_name((const struct sockaddr_in*)addr, sender, 16);
//     printf("on_recv:%s, recv from:%s\r\n", buf->base, sender);

//     int i=0;
//     while(i < nread)
//     {
//         buf->base[i] = toupper(buf->base[i]);
//         i++;
//     }

//     uv_buf_t wrbuf = uv_buf_init(buf->base, nread);

//     uv_udp_send_t send_req;
//     uv_udp_t send_socket;
//     uv_udp_init(loop, &send_socket);

//     uv_udp_send(&send_req, &send_socket, &wrbuf, 1, addr, on_send);

//     if (buf->base)
//     {
//         free(buf->base);
//     }

//     uv_udp_recv_stop(req);
// }

// int main()
// {
//     printf("buliding udp\n");
//     loop = uv_default_loop();

//     uv_udp_t recv_socket;

//     //server端接收数据
//     uv_udp_init(loop, &recv_socket);
//     struct sockaddr_in recv_addr;
//     uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &recv_addr);
//     uv_udp_bind(&recv_socket, (const struct sockaddr *)&recv_addr, UV_UDP_REUSEADDR);
//     uv_udp_recv_start(&recv_socket, alloc_buffer, on_recv);

//     return uv_run(loop, UV_RUN_DEFAULT);
// }