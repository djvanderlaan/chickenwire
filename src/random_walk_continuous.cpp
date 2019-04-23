#include "stepper.h"
#include "chunker.h"
#include "random_walk.h"
#include <iostream>

class RandomWalkDataCont {
  public:
    double x[2];
    double w[2];
    double x_sum;
    double w_sum;
};


class RandomWalkComputationCont {
  public:
    RandomWalkComputationCont(unsigned int nworkers, const VertexList& vertices, 
        const VertexDoubleValues& values, double alpha) : chunks_(vertices, nworkers),
          alpha_(alpha) {
      for (auto p = values.cbegin(); p != values.cend(); ++p) {
        data_[p->first] = { p->second, 0, 1, 0, 0, 0};
      }
    }

    RandomWalkComputationCont(unsigned int nworkers, const VertexList& vertices, 
        const VertexWDoubleValues& values, double alpha) : chunks_(vertices, nworkers),
          alpha_(alpha) {
      for (auto p = values.cbegin(); p != values.cend(); ++p) {
        data_[p->first] = { p->second.value*p->second.weight, 0, p->second.weight, 0, 0, 0};
      }
    }


    bool operator()(int step, unsigned int id, unsigned int nworkers) {
      const unsigned int prev = (step + 0) % 2;
      const unsigned int cur  = (step + 1) % 2;
      bool stop = true;
      for (auto p = chunks_.begin(id); p != chunks_.end(id); ++p) {
        RandomWalkDataCont& d = data_[p->first];
        double y_prev = d.x_sum/d.w_sum;
        d.x[cur] = 0.0;
        d.w[cur] = 0.0;
        EdgeList edges = p->second.edges();
        for (auto q = edges.begin(); q != edges.end(); ++q) {
          const RandomWalkDataCont& d2 = data_[q->dst()];
          d.w[cur] += q->weight() * d2.w[prev];
          d.x[cur] += q->weight() * d2.x[prev];
        }
        d.x_sum  += (1-alpha_) * d.x[cur];
        d.w_sum  += (1-alpha_) * d.w[cur];
        d.w[cur] *= alpha_;
        d.x[cur] *= alpha_;
        stop &= std::abs(d.x_sum/d.w_sum - y_prev) < 1E-5;
      }
      return stop;
    }

    RandomWalkResult result() const {
      RandomWalkResult res(data_.size(), 1);
      for (size_t i = 0; i < chunks_.nchunks(); ++i) {
        for (auto p = chunks_.begin(i); p != chunks_.end(i); ++p) {
          const RandomWalkDataCont& d = data_.at(p->first);
          res.set(p->first, 0, d.x_sum/d.w_sum);
        }
      }
      return res;
    }


  private:
    std::unordered_map<VertexID, RandomWalkDataCont> data_;
    Chunker<VertexList> chunks_;
    double alpha_;
};


RandomWalkResult random_walk_continuous(const Graph& graph, const VertexDoubleValues& vertex_values, 
    double alpha, unsigned int nworkers) {
  if (nworkers == 0) nworkers = determine_nthreads(graph.vertices().size());
  RandomWalkComputationCont computation(nworkers, graph.vertices(), vertex_values, alpha);
  Stepper<RandomWalkComputationCont> stepper(computation);
  stepper.run(nworkers, 100);
  std::cout << "Terminating iteration after step " << stepper.step() << ".\n";
  return computation.result();
}

RandomWalkResult random_walk_continuous(const Graph& graph, const VertexWDoubleValues& vertex_values, 
    double alpha, unsigned int nworkers) {
  if (nworkers == 0) nworkers = determine_nthreads(graph.vertices().size());
  RandomWalkComputationCont computation(nworkers, graph.vertices(), vertex_values, alpha);
  Stepper<RandomWalkComputationCont> stepper(computation);
  stepper.run(nworkers, 100);
  std::cout << "Terminating iteration after step " << stepper.step() << ".\n";
  return computation.result();
}

