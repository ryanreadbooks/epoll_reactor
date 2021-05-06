#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <iostream>
#include <memory>

#include "listen_handler.h"
#include "socket_handler.h"
#include "reactor.h"

using std::shared_ptr;


ListenHandler::ListenHandler(): listenfd(-1) {}

ListenHandler::~ListenHandler() {
    int ret = close(this->listenfd);
    if (ret == -1) {
        perror("close listenfd error");
    }
}


/**
 * 设置初始监听socket的属性
 * 
 * 参数：
 *  port：使用哪一个端口
*/
bool ListenHandler::listen_on(int port) {
    if (0 > (this->listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0))) {
        perror("could not create listen socket fd");
        return false;
    }
    printf("listen fd = %d\n", this->listenfd);
    // 绑定地址
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(this->listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind error");
        return false;
    }
    // 设置端口可重用
    int socktopt = 1;
    ret = setsockopt(this->listenfd, SOL_SOCKET, SO_REUSEADDR, &socktopt, sizeof(socktopt));
    if (ret < 0) {
        perror("setsocketopt error");
        return false;
    }

    // 准备监听
    ret = listen(this->listenfd, 128);
    if (ret < 0) {
        perror("listen error");
        return false;
    }

    return true;
}


/**
 * 读事件，这里是有客户端连接过来了，才会调用到这个方法
 * 建立和对端的连接，并且将连接成功的socket注册仅epoll instance中，进行监听
*/
void ListenHandler::handle_read() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int clifd = accept(this->listenfd, (struct sockaddr*)&addr, &len);
    printf("connection from %s: %d estabilshed\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    // 建立这个文件描述符的EventHanlder处理对象
    // shared_ptr<SockHandler> p_handler = std::make_shared<SockHandler>(clifd);
    SockHandler* p_handler = new SockHandler(clifd);
    set_non_blocking(p_handler->get_handle());
    // SockHandler sockhandler(clifd);
    // set_non_blocking(sockhandler.get_handle());
    // 注册进epoll instance中，通过Reactor对象来实现
    Reactor& reactor = Reactor::get_instance();
    if (reactor.register_handler(p_handler, ReadEvent) == 0) {
        // 注册读事件处理
        std::cout << "register fd = " << clifd << " succeed\n";
    } else { 
        std::cout << "register fd = " << clifd << " failed\n";
    }
}


void ListenHandler::handle_write() {

}


void ListenHandler::handle_error() {

}


void ListenHandler::set_non_blocking(int fd) {
    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(this->listenfd, F_SETFL, flag);
}