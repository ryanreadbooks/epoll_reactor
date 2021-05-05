#include <iostream>

#include "reactor.h"
#include "listen_handler.h"


int main(int argc, char const *argv[])
{
    
    ListenHandler handler;
    handler.listen_on(9527);
    Reactor& reactor = Reactor::get_instance();
    if (reactor.register_handler(&handler, ReadEvent) == 0) {
        // 监听处理器注册到reactor中
        std::cout << "register for listener succeeded\n";
    } else {
        std::cout << "register for listener failed\n";
        exit(1);
    }

    while (true)
    {
        reactor.event_loop(-1); // 阻塞
    }
    
    return 0;
}
