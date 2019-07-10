#include "random_walk.h"
#include "stepper.h"
#include "chunker.h"

#include <iostream>
#include <iomanip>

class RandomWalkContinuousComputation {
  public:
    RandomWalkContinuousComputation(unsigned int nworkers, const VertexList& vertices, 
        const VertexDoubleValues& values, double alpha) : 
          vertices_(vertices), chunks_(vertices, nworkers), alpha_(alpha), 
          xsum_(vertices.size(), 0.0), wsum_(vertices.size(), 0.0),
          x1_(values), x2_(vertices.size()), 
          w1_(vertices.size(), 1.0), w2_(vertices.size()) {
    }

    //RandomWalkContinuousComputation(unsigned int nworkers, const VertexList& vertices, 
        //const VertexWDoubleValues& values, double alpha) : chunks_(vertices, nworkers),
          //alpha_(alpha) {
      //for (auto p = values.cbegin(); p != values.cend(); ++p) {
        //data_[p->first] = { p->second.value*p->second.weight, 0, p->second.weight, 0, 0, 0};
      //}
    //}


    bool operator()(int step, unsigned int id, unsigned int nworkers) {
      const bool even = (step % 2) == 0;
      //std::cout << "Step=" << step << "\t\t" << even << std::endl;
      VertexDoubleValues& x_prev = even ? x1_ : x2_;
      VertexDoubleValues& x_cur  = even ? x2_ : x1_;
      VertexDoubleValues& w_prev = even ? w1_ : w2_;
      VertexDoubleValues& w_cur  = even ? w2_ : w1_;

      bool stop = true;
      for (size_t i = chunks_.ibegin(id), iend = chunks_.iend(id); i < iend; ++i) {
        double y_prev = xsum_[i]/wsum_[i];// d.x[cur] = 0.0;
        x_cur[i] = 0.0;
        w_cur[i] = 0.0;
        EdgeList edges = vertices_[i].edges();

        for (auto q = edges.begin(), qend = edges.end(); q != qend; ++q) {
          size_t j = q->dst();
          w_cur[i] += q->weight() * w_prev[j];
          x_cur[i] += q->weight() * x_prev[j];
        }
        xsum_[i] += (1-alpha_) * x_cur[i];
        wsum_[i] += (1-alpha_) * w_cur[i];
        w_cur[i] *= alpha_;
        x_cur[i] *= alpha_;
        stop &= std::abs(xsum_[i]/wsum_[i] - y_prev) < 1E-5;
      }
      return stop;
    }

    VertexDoubleValues result() const {
      VertexDoubleValues res(vertices_.size());
      for (size_t i = 0, iend = vertices_.size(); i < iend; ++i) 
        res[i] = xsum_[i] / wsum_[i];
      return res;
    }


  private:
    const VertexList& vertices_;
    Chunker<VertexList> chunks_;
    double alpha_;
    VertexDoubleValues xsum_, wsum_, x1_, x2_, w1_, w2_;
};

VertexDoubleValues random_walk_continuous(const Graph& graph, 
    const VertexDoubleValues& vertex_values, 
    double alpha, unsigned int nworkers) {
  if (nworkers == 0) nworkers = determine_nthreads(graph.vertices().size());
  RandomWalkContinuousComputation computation(nworkers, graph.vertices(), vertex_values, alpha);
  Stepper<RandomWalkContinuousComputation> stepper(computation);
  //std::cout << "FOO" << std::endl;
  stepper.run(nworkers, 100);
  //std::cout << "FOO" << std::endl;
  //std::cout << "Terminating iteration after step " << stepper.step() << ".\n";
  return computation.result();
}


//RandomWalkResult random_walk_continuous(const Graph& graph, const VertexDoubleValues& vertex_values, 
    //double alpha, unsigned int nworkers) {
  //if (nworkers == 0) nworkers = determine_nthreads(graph.vertices().size());
  //RandomWalkComputationCont computation(nworkers, graph.vertices(), vertex_values, alpha);
  //Stepper<RandomWalkComputationCont> stepper(computation);
  //stepper.run(nworkers, 100);
  ////std::cout << "Terminating iteration after step " << stepper.step() << ".\n";
  //return computation.result();
//}

