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


/*typedef std::pair<size_t, size_t> VectorRange;
typedef std::vector<VectorRange> VectorRanges;

template<typename T>
VectorRanges chunk_vector(const T& vector, size_t nchunks) {
  VectorRanges res;
  size_t size = vector.size();
  size_t chunk_size = std::max(size/nchunks, 1UL);
  size_t pos = 0;
  for (size_t i = 0; i < nchunks; ++i) {
    size_t remain = size - pos;
    size_t current_chunk_size = std::min(chunk_size, remain);
    res.push_back(VectorRange(pos, pos+current_chunk_size));
    pos += current_chunk_size;
  }
  return res;
}
*/

class Computation2 {
  public:
    Computation2(unsigned int nworkers, const std::vector<double>& x) : total_(x.size()), x1_(x), x2_(x), 
        //chunks_(chunk_vector(x, nworkers)) {
        chunks_(x, nworkers) {
    }

    bool operator()(int step, int id, int nworkers) {
      //std::cout << id << ": Starting computation\n";
      //std::this_thread::sleep_for(std::chrono::seconds(id+1));
      bool odd = (step % 2) == 1;
      double size = total_.size();
      const std::vector<double>& x = odd ? x2_ : x1_;
      std::vector<double>& y = odd ? x1_ : x2_;
      //VectorRange r = chunks_[id];
      //for (size_t i = r.first; i < r.second; ++i) {
      for (size_t i = chunks_.ibegin(id); i < chunks_.iend(id); ++i) {
        double total = 0.0;
        for (auto p = x.cbegin(); p != x.cend(); ++p) {
          total += (*p)/size;
        } 
        double alpha = std::pow(0.85, step);
        total_[i] += (1-alpha) * total;
        y[i] = total;
      }
      //std::cout << id << "/" << nworkers << ": Finished computation\n";
      return step > 30;
    }

    const std::vector<double>& result() { return total_;}

  private:
    std::vector<double> total_;
    std::vector<double> x1_;
    std::vector<double> x2_;
    Chunker<std::vector<double>> chunks_;
    //VectorRanges chunks_;
};


int main(int argc, char* argv[]) {
  std::vector<double> x;
  size_t n = 4E3;
  for (int i = 0; i < n; ++i) {
    x.push_back((double)i/(double)n);
  }
  //std::cout << -(n/2) << " - " << (n/2) << "\n";
  //std::cout << x.size() << "\n";

  unsigned int nworkers = 1;
  Computation2 computation(nworkers, x);
  Stepper<Computation2> stepper(computation);
  stepper.run(nworkers);

  //int i = 0;
  //for (auto p = computation.result().cbegin(); p != computation.result().cend(); ++p) {
    //std::cout << (*p) << ";";
    //if (i > 100) break;
  //}

  /*
  VectorRanges r = chunk_vector(x, 4);
  for (auto p = r.begin(); p != r.end(); ++p) {
    std::cout << p->first << " - " << p->second << std::endl;
  }
  */

  return 0;
} 

