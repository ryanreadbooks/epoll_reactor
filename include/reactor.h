#ifndef REACTOR_REACTOR_H
#define REACTOR_REACTOR_H

#include <map>
#include <memory>

#include "epoll_demultiplexer.h"
#include "event_handler.h"
#include "event.h"

class ReactorImpl {
    public:
        ReactorImpl();
        ~ReactorImpl();

        typedef std::map<Handle, EventHandler*>::iterator Iterator;

        int register_handler(EventHandler* handler, Event evt);
        void remove_handler(EventHandler* handler);
        void event_loop(int timeout = 0);

    private:
        std::unique_ptr<EventDemultiplexer> demultiplexer;  // 多路复用器
};


class Reactor {
    private:
        Reactor();
        Reactor(const Reactor& r);
        Reactor& operator=(const Reactor& r);

    public:
        static Reactor& get_instance();
        int register_handler(EventHandler* handler, Event evt);
        void remove_handler(EventHandler* handler);
        void event_loop(int timeout = 0);

    private:
        std::unique_ptr<ReactorImpl> impl;
        static Reactor instance_;
};

#endif