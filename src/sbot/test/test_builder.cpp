#include <gtest/gtest.h>
#include <httplib.h>

#include "sbot/builder.h"

TEST(SBOT, send_command) {
  Builder builder;
  builder.send_command(std::vector<double>(6,10));
}

TEST(SBOT, send_command_queue) {
  Builder builder;
  std::vector<std::vector<double>> jointQueue;
  for (size_t i=0; i<10; ++i) {
    std::vector<double> joint = std::vector<double>(6, 90 * sin(M_PI/10*i));
    jointQueue.push_back(joint);
  }
  builder.send_command_queue(jointQueue);
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
