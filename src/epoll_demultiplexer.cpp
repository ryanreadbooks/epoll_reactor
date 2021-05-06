#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <vector>
#include <map>
#include <cstdio>
#include <sstream>

#include "epoll_demultiplexer.h"
#include "event_demultiplexer.h"
#include "event.h"
#include "event_handler.h"

using namespace std;

const int EpollDemultiplexer::s_max_event = 1024;   // 静态变量的初始化

/**
 * 构造函数
 * 创建出一个epoll实例
*/
EpollDemultiplexer::EpollDemultiplexer() :epoll_fd(epoll_create(s_max_event)) {
    evs.reserve(this->s_max_event);
}

/**
 * 析构函数
 * 关闭对应的epoll实例
*/
EpollDemultiplexer::~EpollDemultiplexer() {
    int ret = close(this->epoll_fd);
    if (ret == -1) {
        perror("close epoll fd error");
    }
}


/**
 * 往epoll示例中注册文件需要被监听的文件描述符
 * 
 * 参数：
 *  handler：事件处理器对象指针，可以传入Handler类的子类，子类里面包含一个属性表示要监听的文件描述符。通过get_handle()方法获取
 *  evt：在文件描述符上监听什么事件
*/
int EpollDemultiplexer::register_handler(EventHandler* handler, Event evt) {
    struct epoll_event ev;
    ev.data.ptr = (void*) handler;

    // 判断需要注册什么事件
    if (evt & ReadEvent) {  // 如果需要监听读事件
        ev.events = EPOLLIN;   // 读事件
        ev.events |= EPOLLET;   // 设定边缘触发
    }  
    if (evt & WriteEvent) { // 如果需要监听写事件
        ev.events |= EPOLLOUT;  // 写事件
    }
    int ret = epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, handler->get_handle(), &ev);
    // 错误判断
    if (ret != 0) {
        if (errno == ENOENT) {  /* No such file or directory */
            stringstream ss;
            ss << "epoll ctl add fd " << handler->get_handle() << " error, now try again";
            perror(ss.str().c_str());
            if (0 != epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, handler->get_handle(), &ev)) {
                ss.clear();
                ss << "can not register fd " << handler->get_handle() << " into the epoll instance";
                perror(ss.str().c_str());
                return -errno;  // 该函数返回负数，表示注册失败
            }
        }
    }
    return 0;   // 注册成功返回0
}


/**
 * 关键的方法
 * 内部使用epoll_wait等待事件发生
 * 逐一处理读事件、写事件和出错事件
*/
int EpollDemultiplexer::wait_event(int timeout) {
    // 系统调用epoll_wait等待时间发生
    // 返回有时间发生的文件描述符个数
    // "events" parameter is a buffer that will contain triggered events
    // 第二个参数是数组首地址，也是存放准备就绪的文件描述符的struct epoll_event的缓冲区，是由内核复制过来的
    int nready = epoll_wait(this->epoll_fd, &(this->evs[0]), this->s_max_event, timeout);
    // 开始遍历处理num个文件描述符，并且需要注意判断它们上面发生的是什么事件
    for (int i = 0; i < nready; ++i) {
        EventHandler* handler = (EventHandler*) evs[i].data.ptr;    // 在register_handler中存进ptr的就是handler，所以可以强转
        // 判断上面发生了什么事件，逐一处理
        if (evs[i].events & EPOLLIN) {
            // 处理读事件
            // cout << "wait event read event\n";
            handler->handle_read();
        }
        // 读写时间有可能同时存在，因此不是非此即彼
        if (evs[i].events & EPOLLOUT) {
            // 处理写事件
            // cout << "wait event write event\n";
            handler->handle_write();
        }
        // EPOLLRDHUP表示读关闭了
        // EPOLLHUP表示读写都关闭了
        if (evs[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
            // perror("wait event error");
            // 出错的情况
            handler->handle_error();
        }
    }
    return nready;  // 返回准备就绪的文件描述符个数
}


/**
 * 从epoll instance中移除不再需要监听的文件描述符
*/
int EpollDemultiplexer::remove_handler(Handle handle) {
    struct epoll_event ev;
    if (0 != epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, handle, &ev)) {
        stringstream ss;
        ss << "can not remove " << handle << " from epoll instance";
        perror(ss.str().c_str());
        return -errno;
    }
    return 0;
}