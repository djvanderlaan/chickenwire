#include "random_walk.h"
#include "stepper.h"
#include "chunker.h"

class RandomWalkContinuousComputation {
  public:
    RandomWalkContinuousComputation(unsigned int nworkers, const VertexList& vertices, 
        const VertexDoubleValues& values, double alpha) : 
          vertices_(vertices), chunks_(vertices, nworkers), alpha_(alpha), 
          xsum_(vertices.size(), 0.0), x1_(values), x2_(vertices.size()), 
          wsum_(vertices.size(), 0.0), w1_(vertices.size(), 1.0), w2_(vertices.size()),
          precision_(1E-5) {
    }

    RandomWalkContinuousComputation(unsigned int nworkers, const VertexList& vertices, 
        const VertexDoubleValues& values, const VertexWeights& weights, double alpha) : 
          vertices_(vertices), chunks_(vertices, nworkers), alpha_(alpha), 
          xsum_(vertices.size(), 0.0), x1_(values), x2_(vertices.size()), 
          wsum_(vertices.size(), 0.0), w1_(weights), w2_(vertices.size()),
          precision_(1E-5) {
      // x1 needs to be values * weights
      for (auto p = x1_.begin(), q = w1_.begin(), end = x1_.end(); p != end; ++p, ++q) {
        (*p) = (*p) * (*q);
      }
    }

    void precision(double precision) { precision_ = precision;}
    double precision() const { return precision_;}

    bool operator()(int step, unsigned int id, unsigned int nworkers) {
      const bool even = (step % 2) == 0;
      VertexDoubleValues& x_prev = even ? x1_ : x2_;
      VertexDoubleValues& x_cur  = even ? x2_ : x1_;
      VertexWeights&      w_prev = even ? w1_ : w2_;
      VertexWeights&      w_cur  = even ? w2_ : w1_;

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
        stop &= std::abs(xsum_[i]/wsum_[i] - y_prev) < precision_;
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
    VertexDoubleValues xsum_, x1_, x2_;
    VertexWeights wsum_, w1_, w2_;
    double precision_;
};

VertexDoubleValues random_walk_continuous(const Graph& graph, 
    const VertexDoubleValues& vertex_values, 
    double alpha, unsigned int nworkers, unsigned int nstep_max, double precision,
    int* nsteps) {
  if (nworkers == 0) nworkers = determine_nthreads(graph.vertices().size());
  RandomWalkContinuousComputation computation(nworkers, graph.vertices(), vertex_values, alpha);
  computation.precision(precision);
  Stepper<RandomWalkContinuousComputation> stepper(computation);
  stepper.run(nworkers, nstep_max);
  if (nsteps) (*nsteps) = stepper.step();
  return computation.result();
}

VertexDoubleValues random_walk_continuous(const Graph& graph, 
    const VertexDoubleValues& vertex_values, const VertexWeights& vertex_weights,
    double alpha, unsigned int nworkers, unsigned int nstep_max, double precision,
    int* nsteps) {
  if (nworkers == 0) nworkers = determine_nthreads(graph.vertices().size());
  RandomWalkContinuousComputation computation(nworkers, graph.vertices(), vertex_values, 
    vertex_weights, alpha);
  computation.precision(precision);
  Stepper<RandomWalkContinuousComputation> stepper(computation);
  stepper.run(nworkers, nstep_max);
  if (nsteps) (*nsteps) = stepper.step();
  return computation.result();
}

// There used to be a seperate implementation for categorical values. However, 
// the performance of that algorithm was practically the same as using the
// the algorithm for continuous variables for each category seperately. As,
// maintaining one version of the algorithm is easier it was decided to drop
// that version. It should be somewhere in the git archives. 

inline VertexType ncategorical_values(const VertexCategoricalValues& values) {
  return *(std::max_element(values.cbegin(), values.cend()));
}


RandomWalkResult random_walk_categorical(const Graph& graph, 
    const VertexCategoricalValues& vertex_values, 
    double alpha, unsigned int nworkers, unsigned int nstep_max, double precision,
    int* nsteps) {
  const VertexType nvalues = ncategorical_values(vertex_values);
  RandomWalkResult res;
  VertexDoubleValues values(vertex_values.size());
  for (VertexType type = 0; type <= nvalues; ++type) {
    for (size_t i = 0; i < values.size(); ++i) {
      values[i] = vertex_values[i] == type ? 1 : 0;
    }
    res.push_back(random_walk_continuous(graph, values, alpha, 
      nworkers, nstep_max, precision, nsteps));
  }
  return res;
}

RandomWalkResult random_walk_categorical(const Graph& graph, 
    const VertexCategoricalValues& vertex_values, const VertexWeights& vertex_weights, 
    double alpha, unsigned int nworkers, unsigned int nstep_max, double precision, 
    int* nsteps) {
  const VertexType nvalues = ncategorical_values(vertex_values);
  RandomWalkResult res;
  VertexDoubleValues values(vertex_values.size());
  for (VertexType type = 0; type <= nvalues; ++type) {
    for (size_t i = 0; i < values.size(); ++i) {
      values[i] = vertex_values[i] == type ? 1 : 0;
    }
    res.push_back(random_walk_continuous(graph, values, vertex_weights, alpha, 
      nworkers, nstep_max, precision, nsteps));
  }
  return res;
}

