#ifndef _REACTOR_EVENTDEMULTIPLEXER_H_
#define _REACTOR_EVENTDEMULTIPLEXER_H_

#include <iostream>
#include <sys/epoll.h>
#include <map>
#include <vector>

#include "event_handler.h"
#include "event.h"


/**
 * 事件多路分发器
 * 抽象类，定义基本的操作
 * 具体的实现有子类完成
*/
class EventDemultiplexer {
    public:
        EventDemultiplexer() {}
        virtual ~EventDemultiplexer() {}
        virtual int wait_event(int timeout) = 0;
        virtual int register_handler(EventHandler* handler, Event evt) = 0;
        virtual int remove_handler(Handle handle) = 0;

};

#endif