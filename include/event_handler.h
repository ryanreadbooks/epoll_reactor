#ifndef _REACTOR_HANDLE_H_
#define _REACTOR_HANDLE_H_

typedef int Handle; // 相当于文件描述符

/**
 * 事件处理器
 * 作为一个抽象类，定义共用的处理方法
 */
class EventHandler {
public:
  EventHandler() {}
  virtual ~EventHandler() {}
  virtual Handle get_handle() const = 0;
  virtual void handle_read() = 0;
  virtual void handle_write() = 0;
  virtual void handle_error() = 0;
};

#endif