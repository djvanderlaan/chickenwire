#include "stepper.h"
#include "chunker.h"
#include "random_walk.h"
#include <iostream>

class RandomWalkDataCat {
  public:
    double* x;
    double* w;
    double* x_sum;
    double* w_sum;
};


class RandomWalkComputationCat {
  public:
    RandomWalkComputationCat(unsigned int nworkers, const VertexList& vertices, 
        const VertexCategoricalValues& values, VertexType nvalues, double alpha) : chunks_(vertices, nworkers),
          nvalues_(nvalues), alpha_(alpha) {
      // For each vertex we need to store the present and previous values of both weight
      // and x. x and w consist of nvalues values.
      size_t n = nvalues * 6UL * vertices.size();
      store_ = std::unique_ptr<double []>(new double[n]);
      double* ptr = store_.get();
      for (size_t i = 0; i < n; ++i, ++ptr) (*ptr) = 0.0;
      ptr = store_.get();
      for (auto p = values.cbegin(); p != values.cend(); ++p) {
        double* w = ptr + (2UL*nvalues);
        data_[p->first] = {ptr, w, ptr + (4UL*nvalues), ptr + (5UL*nvalues)};
        ptr[p->second] = 1.0;
        for (size_t i = 0; i < nvalues; ++i, ++w) (*w) = 1.0;
        ptr += nvalues * 6UL;
        *((data_[p->first]).x_sum) = 0.0;
        *((data_[p->first]).w_sum) = 0.0;
      }
    }

    bool operator()(int step, unsigned int id, unsigned int nworkers) {
      bool odd = (step % 2) == 1;
      size_t prev = odd ? nvalues_ : 0;
      size_t cur  = odd ? 0 : nvalues_;
      std::unique_ptr<double[]> y_prev(new double[nvalues_]);

      bool stop = true;
      for (auto p = chunks_.begin(id); p != chunks_.end(id); ++p) {
        RandomWalkDataCat& d = data_[p->first];
        for (VertexType i = 0; i < nvalues_; ++i) {
          y_prev[i] = d.x_sum[i]/d.w_sum[i];
          d.x[cur + i] = 0.0;
          d.w[cur + i] = 0.0;
        }
        EdgeList edges = p->second.edges();
        for (auto q = edges.begin(); q != edges.end(); ++q) {
          const RandomWalkDataCat& d2 = data_[q->dst()];
          for (VertexType i = 0; i < nvalues_; ++i) {
            d.w[cur+i] += q->weight() * d2.w[prev+i];
            d.x[cur+i] += q->weight() * d2.x[prev+i];
          }
        }
        for (VertexType i = 0; i < nvalues_; ++i) {
          d.x_sum[i]  += (1-alpha_) * d.x[cur+i];
          d.w_sum[i]  += (1-alpha_) * d.w[cur+i];
          d.w[cur+i] *= alpha_;
          d.x[cur+i] *= alpha_;
          stop &= std::abs(d.x_sum[i]/d.w_sum[i] - y_prev[i]) < 1E-5;
        }
      }
      return stop;
    }

    void print() {
      auto p = data_.cbegin();
      for (VertexID i = 0; i < 10; ++i, ++p) {
        const RandomWalkDataCat& d = p->second;
        std::cout << p->first << ": \n";
        for (VertexType j = 0; j < nvalues_; ++j) {
          std::cout << "\t\t" << j << " " << d.x_sum[j] << " w = " << d.w_sum[j] << "\n";
        }
      }
    }

  private:
    std::unordered_map<VertexID, RandomWalkDataCat> data_;
    Chunker<VertexList> chunks_;
    VertexType nvalues_;
    std::unique_ptr<double[]> store_;
    double alpha_;
};

inline VertexType get_max_type(const VertexCategoricalValues& values) {
  VertexType max = 0;
  for (auto p = values.begin(); p != values.end(); ++p) {
    if (p->second > max) max = p->second;
  }
  return max;
}

void random_walk_threaded_categorical(const Graph& graph, const VertexCategoricalValues& vertex_values, 
    double alpha) {

  unsigned int nworkers = determine_nthreads(graph.vertices().size());
  VertexType nvalues = get_max_type(vertex_values) + 1;
  RandomWalkComputationCat computation(nworkers, graph.vertices(), vertex_values, nvalues, alpha);
  Stepper<RandomWalkComputationCat> stepper(computation);
  stepper.run(nworkers, 100);
  std::cout << "Terminating iteration after step " << stepper.step() << ".\n";
  //computation.print();
}

