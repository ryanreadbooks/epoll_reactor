#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket_handler.h"



SockHandler::SockHandler(Handle fd) : fd_(fd), buf(new char[MAX_SIZE]) {
    // printf("sockhandler constructor\n");
    memset(buf, 0, MAX_SIZE);
}


SockHandler::~SockHandler() {
    // printf("sockhandler destructor\n");
    close(this->fd_);
    delete buf;
}


Handle SockHandler::get_handle() const {return this->fd_;}


void SockHandler::handle_read() {
    // 读事件
    // 从socket fd中读取
    // 这里先实现最简单的单次读取
    // echo服务器，业务逻辑
    int n = read(this->fd_, this->buf, MAX_SIZE);
    this->buf[n] = '\0';
    printf("read n = %d, content = %s\n", n, this->buf);
    if (n > 0) {
        write(this->fd_, buf, strlen(buf)); // 内容原封不动写回去
    } else if (n == 0){ 
        // 对端关闭
        printf("client %d closed\n", this->fd_);
        close(this->fd_);
    } else if (n == -1){
        // 出错
        handle_error();
    }
}


void SockHandler::handle_write() {

}


void SockHandler::handle_error() {
    // 出错关闭自己所使用的文件描述符
    // close(fd_);
    // perror("sockHandler");
}