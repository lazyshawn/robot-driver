#include <gtest/gtest.h>
#include <httplib.h>

#include "sbot/builder.h"

TEST(SBOT, send_command) {
  Builder builder;
  builder.set_joint_state(std::vector<double>(6,10));
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
