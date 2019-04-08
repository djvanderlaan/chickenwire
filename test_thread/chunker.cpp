#include <vector>
#include <iostream>
#include <iomanip>
#include "chunker.h"

int main(int argc, char* argv[]) {
  size_t n = 40;
  std::vector<double> v;
  for (size_t i = 0; i < n; ++i) {
    v.push_back(i);
  }

  Chunker<std::vector<double>> chunks(v, 4);

  size_t j = 0;
  for (size_t i = 0; i < chunks.nchunks(); ++i) {
    std::cout << "Chunk " << i << std::endl;
    std::cout << chunks.ibegin(i) << "-" << chunks.iend(i) << std::endl;
    for (auto p = chunks.begin(i); p != chunks.end(i); ++p) {
      std::cout << *p << std::endl;
      ++j;
      if (j > n) break;
    }
  }
}

