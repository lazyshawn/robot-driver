#include "sbot/builder.h"

int main(int argc, char** argv) {
  Builder builder;

  builder.moveJ({1,1,1,1,1,1}, 0.2);

  return 0;
}
