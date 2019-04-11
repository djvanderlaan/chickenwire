#include <iostream>
#include <iomanip>
#include <cmath>

#include "stepper.h"
#include "chunker.h"

class Computation {
  public:
    bool operator()(int step, int id, int nthreads) {
      std::cout << id << ": Starting computation\n";
      std::this_thread::sleep_for(std::chrono::seconds(id+1));
      std::cout << id << "/" << nthreads << ": Finished computation\n";
      return step > 3;
    }
};


class Computation2 {
  public:
    Computation2(unsigned int nworkers, const std::vector<double>& x) : total_(x.size()), x1_(x), x2_(x), 
        chunks_(x, nworkers) {
    }

    bool operator()(int step, int id, int nworkers) {
      std::cout << "Step=" << step << std::endl;
      bool odd = (step % 2) == 1;
      double size = total_.size();
      const std::vector<double>& x = odd ? x2_ : x1_;
      std::vector<double>& y = odd ? x1_ : x2_;
      for (size_t i = chunks_.ibegin(id); i < chunks_.iend(id); ++i) {
        double total = 0.0;
        for (auto p = x.cbegin(); p != x.cend(); ++p) {
          total += (*p)/size;
        } 
        double alpha = std::pow(0.85, step);
        total_[i] += (1-alpha) * total;
        y[i] = total;
      }
      return step > 30;
    }

    const std::vector<double>& result() { return total_;}

  private:
    std::vector<double> total_;
    std::vector<double> x1_;
    std::vector<double> x2_;
    Chunker<std::vector<double>> chunks_;
};


int main(int argc, char* argv[]) {
  std::vector<double> x;
  size_t n = 4E3;
  for (int i = 0; i < n; ++i) {
    x.push_back((double)i/(double)n);
  }

  unsigned int nworkers = 2;
  Computation2 computation(nworkers, x);
  Stepper<Computation2> stepper(computation);
  stepper.run(nworkers);

  //int i = 0;
  //for (auto p = computation.result().cbegin(); p != computation.result().cend(); ++p) {
    //std::cout << (*p) << ";";
    //if (i > 100) break;
  //}

  return 0;
} 

