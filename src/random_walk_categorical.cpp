#include "stepper.h"
#include "chunker.h"
#include "random_walk.h"
//#include <iostream>


class RandomWalkComputationCat {
  public:
    RandomWalkComputationCat(unsigned int nworkers, const VertexList& vertices, 
        const VertexCategoricalValues& values, VertexType nvalues, double alpha) : chunks_(vertices, nworkers),
          nvalues_(nvalues), alpha_(alpha), store_(vertices.size(), nvalues*6UL) {
      for (auto p = values.cbegin(); p != values.cend(); ++p) {
        double* ptr = store_.insert(p->first);
        ptr[p->second] = 1.0;
        double* w = ptr + (2UL*nvalues);
        for (size_t i = 0; i < nvalues; ++i, ++w) (*w) = 1.0;
      }
    }

    RandomWalkComputationCat(unsigned int nworkers, const VertexList& vertices, 
        const VertexWCategoricalValues& values, VertexType nvalues, double alpha) : chunks_(vertices, nworkers),
          nvalues_(nvalues), alpha_(alpha), store_(vertices.size(), nvalues*6UL) {
      for (auto p = values.cbegin(); p != values.cend(); ++p) {
        double* ptr = store_.insert(p->first);
        ptr[p->second.value] = p->second.weight;
        double* w = ptr + (2UL*nvalues);
        for (size_t i = 0; i < nvalues; ++i, ++w) (*w) = p->second.weight;
      }
    }

    bool operator()(int step, unsigned int id, unsigned int nworkers) {
      bool odd = (step % 2) == 1;
      size_t prev = odd ? nvalues_ : 0;
      size_t cur  = odd ? 0 : nvalues_;
      std::unique_ptr<double[]> y_prev(new double[nvalues_]);

      bool stop = true;
      for (auto p = chunks_.begin(id); p != chunks_.end(id); ++p) {
        double* x = store_.getp(p->first);
        double* w = x + (2UL*nvalues_);
        double* x_sum = x + (4UL*nvalues_);
        double* w_sum = x + (5UL*nvalues_);

        for (VertexType i = 0; i < nvalues_; ++i) {
          y_prev[i] = x_sum[i]/w_sum[i];
          x[cur + i] = 0.0;
          w[cur + i] = 0.0;
        }
        EdgeList edges = p->second.edges();
        for (auto q = edges.begin(); q != edges.end(); ++q) {
          double* x2 = store_.getp(q->dst());
          double* w2= x2 + (2UL*nvalues_);
          for (VertexType i = 0; i < nvalues_; ++i) {
            w[cur+i] += q->weight() * w2[prev+i];
            x[cur+i] += q->weight() * x2[prev+i];
          }
        }
        for (VertexType i = 0; i < nvalues_; ++i) {
          x_sum[i]  += (1-alpha_) * x[cur+i];
          w_sum[i]  += (1-alpha_) * w[cur+i];
          w[cur+i] *= alpha_;
          x[cur+i] *= alpha_;
          stop &= std::abs(x_sum[i]/w_sum[i] - y_prev[i]) < 1E-5;
        }
      }
      return stop;
    }

    RandomWalkResult result() const {
      RandomWalkResult res(store_.size(), nvalues_);
      for (size_t i = 0; i < chunks_.nchunks(); ++i) {
        for (auto p = chunks_.begin(i); p != chunks_.end(i); ++p) {
          double* resp = res.insert(p->first);
          const double* x_sum = store_.getp(p->first) + (4UL*nvalues_);
          const double* w_sum = x_sum + nvalues_;
          for (size_t j = 0; j < nvalues_; ++j) 
            resp[j] = x_sum[j]/w_sum[j];
        }
      }
      return res;
    }

    //void print() {
    //  for (VertexID i = 0; i < 10; ++i) {
    //    double* x = store_.getp(i);
    //    double* x_sum = x + (4UL*nvalues_);
    //    double* w_sum = x + (5UL*nvalues_);
    //    std::cout << i << ": \n";
    //    for (VertexType j = 0; j < nvalues_; ++j) {
    //      std::cout << "\t\t" << j << " " << x_sum[j] << " w = " << w_sum[j] << "\n";
    //    }
    //  }
    //}

  private:
    Chunker<VertexList> chunks_;
    VertexType nvalues_;
    double alpha_;
    RandomWalkResult store_;
};

inline VertexType get_max_type(const VertexCategoricalValues& values) {
  VertexType max = 0;
  for (auto p = values.begin(); p != values.end(); ++p) {
    if (p->second > max) max = p->second;
  }
  return max;
}

RandomWalkResult random_walk_categorical(const Graph& graph, const VertexCategoricalValues& vertex_values, 
    double alpha, unsigned int nworkers) {
  if (nworkers == 0) nworkers = determine_nthreads(graph.vertices().size());
  VertexType nvalues = get_max_type(vertex_values) + 1;
  RandomWalkComputationCat computation(nworkers, graph.vertices(), vertex_values, nvalues, alpha);
  Stepper<RandomWalkComputationCat> stepper(computation);
  stepper.run(nworkers, 100);
  //std::cout << "Terminating iteration after step " << stepper.step() << ".\n";
  return computation.result();
}


inline VertexType get_max_type(const VertexWCategoricalValues& values) {
  VertexType max = 0;
  for (auto p = values.begin(); p != values.end(); ++p) {
    if (p->second.value > max) max = p->second.value;
  }
  return max;
}

RandomWalkResult random_walk_categorical(const Graph& graph, const VertexWCategoricalValues& vertex_values, 
    double alpha, unsigned int nworkers) {
  if (nworkers == 0) nworkers = determine_nthreads(graph.vertices().size());
  VertexType nvalues = get_max_type(vertex_values) + 1;
  RandomWalkComputationCat computation(nworkers, graph.vertices(), vertex_values, nvalues, alpha);
  Stepper<RandomWalkComputationCat> stepper(computation);
  stepper.run(nworkers, 100);
  //std::cout << "Terminating iteration after step " << stepper.step() << ".\n";
  return computation.result();
}

