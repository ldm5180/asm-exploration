#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>

constexpr const unsigned maxSize = 1024;

int cArray[maxSize];
std::array<int, maxSize> cppArray;

using CppArrayT = std::array<int, maxSize>;

void cArrayCopy(int *dst, const int *src, unsigned length) {
  for (unsigned i = 0; i < length; ++i) {
    dst[i] = src[i];
  }
}

void cppArrayCopy(CppArrayT& dst, const CppArrayT& src) {
  for (unsigned i = 0; i < src.size(); ++i) {
    dst[i] = src[i];
  }
}

void cppArrayCopyGenerated(CppArrayT &dst, const CppArrayT& src) {
  dst = src;
}

CppArrayT cppArrayRVO(const CppArrayT& src) {
  auto newArr = src;
  return newArr;
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
  for (unsigned i = 0; i < 10000000; ++i) {
    int newCArray[maxSize];
    cArrayCopy(newCArray, cArray, maxSize);
    cAccum += newCArray[i % maxSize];
  }
  auto cArrayEnd = std::chrono::high_resolution_clock::now();

  auto cppArrayBegin = std::chrono::high_resolution_clock::now();
  int cppAccum = 0;
  for (unsigned i = 0; i < 10000000; ++i) {
    CppArrayT newArray;
    cppArrayCopy(newArray, cppArray);
    cppAccum += newArray[i % maxSize];
  }
  auto cppArrayEnd = std::chrono::high_resolution_clock::now();

  int cppGeneratedAccum = 0;
  auto cppArrayAlgoBegin = std::chrono::high_resolution_clock::now();
  for (unsigned i = 0; i < 10000000; ++i) {
    CppArrayT newArray;
    cppArrayCopyGenerated(newArray, cppArray);
    cppGeneratedAccum += newArray[i % maxSize];
  }
  auto cppArrayAlgoEnd = std::chrono::high_resolution_clock::now();

  int cppRVOAccum = 0;
  auto cppArrayRVOBegin = std::chrono::high_resolution_clock::now();
  for (unsigned i = 0; i < 10000000; ++i) {
    CppArrayT newArray = cppArrayRVO(cppArray);
    cppRVOAccum += newArray[i % maxSize];
  }
  auto cppArrayRVOEnd = std::chrono::high_resolution_clock::now();

  std::cout << "C array output:          " << cAccum << "\n";
  std::cout << "C++ array output:        " << cppAccum << "\n";
  std::cout << "C++ array output (gen):  " << cppGeneratedAccum << "\n";
  std::cout << "C++ array output (RVO):  " << cppRVOAccum << "\n";

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
