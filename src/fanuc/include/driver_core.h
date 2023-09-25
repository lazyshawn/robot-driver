#pragma once

#include <iostream>
#include "utils/socket.h"

class RobotDriver {
public:
  fd_set fdSet;
public:
  // https://zhuanlan.zhihu.com/p/72532203
  void connect();
};
