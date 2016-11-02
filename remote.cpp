#include <cstdlib>
#include <ctime>

#include "remote.hpp"

int myFunc() {
  std::srand(std::time(0));
  return std::rand();
}
