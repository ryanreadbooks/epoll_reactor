#include "reactor.h"


ReactorImpl::ReactorImpl() {
    EpollDemultiplexer* plexer = new EpollDemultiplexer;
    demultiplexer.reset(plexer);
}

ReactorImpl::~ReactorImpl() {}

int ReactorImpl::register_handler(EventHandler* handler, Event evt) {
    return demultiplexer->register_handler(handler, evt);
}

void ReactorImpl::remove_handler(EventHandler* handler) {
    demultiplexer->remove_handler(handler->get_handle());
}

void ReactorImpl::event_loop(int timeout) { 
    demultiplexer->wait_event(timeout);
}


/**
 * Reactor实现
*/

Reactor Reactor::instance_;

Reactor& Reactor::get_instance() {
    return instance_;
}

/**
 * private的构造函数
*/
Reactor::Reactor() {
    impl.reset(new ReactorImpl);
}


int Reactor::register_handler(EventHandler* handler, Event evt) {
    return this->impl->register_handler(handler, evt);
}

void Reactor::remove_handler(EventHandler* handler) {
    this->impl->remove_handler(handler);
}

void Reactor::event_loop(int timeout) {
    this->impl->event_loop(timeout);
}