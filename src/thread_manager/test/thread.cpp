#include "utils/interpolate.h"

#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char** argv) {
  using namespace std::chrono;
  // Setup
  std::vector<std::vector<double>> pnt = get_linear_interpolate({0,0,0}, {3,4,5}, 1);
  duration<double, std::milli> circleTime(100);

  // 伺服线程开始，获取当前时刻
  time_point<system_clock, duration<double, std::milli>> stamp = system_clock::now();
  // Set next wake up time point
  stamp += circleTime;

  for (size_t i=0; i<pnt.size(); ++i) {
    time_point<system_clock> cur = system_clock::now();
    if (cur > stamp + circleTime) {
      printf("Warning: phase skipped!\n");
      continue;
    }
    std::cout << cur.time_since_epoch().count() << std::endl;
    stamp = cur + circleTime;

    // Do servo things
    for (auto& j : pnt[i]) {
      std::cout << j << ", ";
    }
    std::cout << std::endl;

    // Finish all job, fall sleep
    std::this_thread::sleep_until(stamp);
  }

  return 0;
}
