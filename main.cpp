#ifdef LOCAL
#include <cstdlib>
#include <ctime>

int myFunc() {
  std::srand(std::time(0));
  return std::rand();
}
#else
#include "remote.hpp"
#endif

int main() {
  int r = myFunc();
  r += myFunc();
  return myFunc();
}
