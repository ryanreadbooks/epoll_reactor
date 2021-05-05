#ifndef EPOLL_DEMULTIPLEXER_H
#define EPOLL_DEMULTIPLEXER_H

#include <vector>
#include "event_demultiplexer.h"

class EpollDemultiplexer: public EventDemultiplexer {
    private:
        static const int s_max_event;   // 最大的事件处理数
        int epoll_fd;   // epoll实例
        std::vector<struct epoll_event> evs; // 保存epoll_event
    
    public:
        EpollDemultiplexer();
        virtual ~EpollDemultiplexer();
        virtual int wait_event(int timeout);
        virtual int register_handler(EventHandler* handler, Event evt);
        virtual int remove_handler(Handle handle);
};

#endif