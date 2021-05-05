#ifndef _REACTOR_LISTEN_HANDLE_H_
#define _REACTOR_LISTEN_HANDLE_H_


#include "event_handler.h"

/**
 * 处理监听事件的时间处理器
*/
class ListenHandler :public EventHandler {
    private:
        Handle listenfd;    // 用于socket监听的文件描述符
    public:
        ListenHandler();
        virtual ~ListenHandler();
        virtual Handle get_handle() const {return listenfd;}
        virtual void handle_read();
        virtual void handle_write();
        virtual void handle_error();

        bool listen_on(int port);

    private:
        void set_non_blocking(int fd);
};


#endif