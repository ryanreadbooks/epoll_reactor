#ifndef REACTOR_SOCK_HANDLER_H_
#define REACTOR_SOCK_HANDLER_H_

#include "event_handler.h"

class SockHandler: public EventHandler {
    public:
        SockHandler(Handle fd);
        virtual ~SockHandler();

        virtual Handle get_handle() const;
        virtual void handle_read();
        virtual void handle_write();
        virtual void handle_error();

    private:
        Handle fd_; // 表示该SockHanlder对应处理的socket文件描述符
        char* buf;  // 缓冲区
        static const int MAX_SIZE = 1024;
};


#endif