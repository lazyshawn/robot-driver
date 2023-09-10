#pragma once

#include <iostream>

// socket 通信
#ifdef WIN32
#include <winsock2.h>
// Need to link with Ws2_32.lib
#pragram comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

class RobotDriver {
public:
  fd_set fdSet;
public:
  // https://zhuanlan.zhihu.com/p/72532203
  void connect();
};
