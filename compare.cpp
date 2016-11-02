#include <algorithm>
#include <array>
#include <string.h>
#include <chrono>
#include <iostream>

constexpr const unsigned maxSize = 102400000;

int cArray1[maxSize];
int cArray2[maxSize];
std::array<int, maxSize> cppArray1;
std::array<int, maxSize> cppArray2;

using CppArrayT = std::array<int, maxSize>;

bool cArrayCompare(const int *a, const int *b, unsigned length) {
  for (unsigned i = 0; i < length; ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

bool cArrayMemCmp(const int* a, const int* b, unsigned length) {
  return memcmp(a, b, length);
}

bool cppArrayCompare(const CppArrayT& a, const CppArrayT& b) {
  for (unsigned i = 0; i < a.size(); ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

bool cppArrayCompareStl(CppArrayT &a, const CppArrayT& b) {
  return a == b;
}

int main() {
  // Initialize the arrays with random data so that the optimizer can't "get its
  // grubby little hands on it".
  std::srand(0);
  for (unsigned i = 0; i < maxSize; ++i) {
    auto v = std::rand();
    cArray1[i] = v;
    cArray2[i] = v;
    cppArray1[i] = v;
  }
  cppArray2 = cppArray1;
  
  auto cArrayBegin = std::chrono::high_resolution_clock::now();
  int cAccum = 0;
  for (unsigned i = 0; i < 1; ++i) {
    int newCArray[maxSize];
    if (cArrayCompare(cArray1, cArray2, maxSize)) {
      cAccum += i % maxSize;
    }
  }
  auto cArrayEnd = std::chrono::high_resolution_clock::now();

  auto cppArrayBegin = std::chrono::high_resolution_clock::now();
  int cppAccum = 0;
  for (unsigned i = 0; i < 1; ++i) {
    CppArrayT newArray;
    if (cArrayMemCmp(cArray1, cArray2, maxSize)) {
      cppAccum += i % maxSize;
    }
  }
  auto cppArrayEnd = std::chrono::high_resolution_clock::now();

  int cppArrayCompareAccum = 0;
  auto cppArrayAlgoBegin = std::chrono::high_resolution_clock::now();
  for (unsigned i = 0; i < 1; ++i) {
    CppArrayT newArray;
    if (cppArrayCompare(cppArray1, cppArray2)) {
      cppArrayCompareAccum += i % maxSize;
    }
  }
  auto cppArrayAlgoEnd = std::chrono::high_resolution_clock::now();

  int cppGenAccum = 0;
  auto cppArrayRVOBegin = std::chrono::high_resolution_clock::now();
  for (unsigned i = 0; i < 1; ++i) {
    if (cppArray1 == cppArray2) {
      cppGenAccum += i % maxSize;
    }
  }
  auto cppArrayRVOEnd = std::chrono::high_resolution_clock::now();

  std::cout << "C array output:          " << cAccum << "\n";
  std::cout << "C++ array output:        " << cppAccum << "\n";
  std::cout << "C++ array output (gen):  " << cppArrayCompareAccum << "\n";
  std::cout << "C++ array output (RVO):  " << cppGenAccum << "\n";

  std::cout << "C array time:            "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   cArrayEnd - cArrayBegin).count() << "\n";
  std::cout << "C++ array time:          "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   cppArrayEnd - cppArrayBegin).count() << "\n";
  std::cout << "C++ array time (gen):    "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   cppArrayAlgoEnd - cppArrayAlgoBegin).count() << "\n";
  std::cout << "C++ array time (RVO):    "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   cppArrayRVOEnd - cppArrayRVOBegin).count() << "\n";
}
