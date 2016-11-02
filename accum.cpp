#include <array>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <numeric>

constexpr const unsigned maxSize = 10240;

int cArray[maxSize];
std::array<int, maxSize> cppArray;

using CppArrayT = std::array<int, maxSize>;

int cArrayValAtIndex(size_t index) { return cArray[index]; }

int cppArrayValAtIndex(size_t index) { return cppArray[index]; }

int cArrayAccum(const int *arr, unsigned length) {
  int accum = 0;
  for (unsigned i = 0; i < length; ++i) {
    accum += arr[i];
  }
  return accum;
}

int cppArrayAccum(const CppArrayT &arr) {
  int accum = 0;
  for (unsigned i = 0; i < arr.size(); ++i) {
    accum += arr[i];
  }
  return accum;
}

int cppArrayAlgoAccum(const CppArrayT &arr) {
  return std::accumulate(arr.begin(), arr.end(), 0);
}

int main() {
  // Initialize the arrays with random data so that the optimizer can't "get its
  // grubby little hands on it".
  std::srand(0);
  for (unsigned i = 0; i < maxSize; ++i) {
    auto v = std::rand();
    cArray[i] = v;
    cppArray[i] = v;
  }

  auto cArrayBegin = std::chrono::high_resolution_clock::now();
  int cAccum = 0;
  for (unsigned i = 0; i < 1000000; ++i) {
    cAccum += cArrayAccum(cArray, maxSize);
  }
  auto cArrayEnd = std::chrono::high_resolution_clock::now();

  auto cppArrayBegin = std::chrono::high_resolution_clock::now();
  int cppAccum = 0;
  for (unsigned i = 0; i < 1000000; ++i) {
    cppAccum += cppArrayAccum(cppArray);
  }
  auto cppArrayEnd = std::chrono::high_resolution_clock::now();

  auto cppArrayAlgoBegin = std::chrono::high_resolution_clock::now();
  int cppAlgoAccum = 0;
  for (unsigned i = 0; i < 1000000; ++i) {
    cppAlgoAccum += cppArrayAlgoAccum(cppArray);
  }
  auto cppArrayAlgoEnd = std::chrono::high_resolution_clock::now();

  std::cout << "C array output:          " << cAccum << "\n";
  std::cout << "C++ array output:        " << cppAccum << "\n";
  std::cout << "C++ array output (algo): " << cppAlgoAccum << "\n";

  std::cout << "C array time:            "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   cArrayEnd - cArrayBegin).count() << "\n";
  std::cout << "C++ array time:          "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   cppArrayEnd - cppArrayBegin).count() << "\n";

  std::cout << "C++ array time (algo):   "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   cppArrayAlgoEnd - cppArrayAlgoBegin).count() << "\n";
}
