#ifndef _REACTOR_EVENT_H
#define _REACTOR_EVENT_H

// 为Event取个别名
typedef unsigned int Event;
enum EventMask {
  ReadEvent  = 0x01,
  WriteEvent = 0x02,
  ErrorEvent = 0x04,
  EventMask  = 0xff
};

#endif