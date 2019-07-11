#include "random_walk.h"
#include "stepper.h"
#include "chunker.h"
#include <algorithm>

inline VertexType ncategorical_values(const VertexCategoricalValues& values) {
  return *(std::max_element(values.cbegin(), values.cend()));
}


class RandomWalkCategoricalComputation {
  public:
    RandomWalkCategoricalComputation(unsigned int nworkers, const VertexList& vertices, 
        const VertexCategoricalValues& values, double alpha) : 
          vertices_(vertices), chunks_(vertices, nworkers), alpha_(alpha) {
      nvalues_ = ncategorical_values(values) + 1;
      size_t size = vertices.size();
      for (unsigned int i = 0; i < nvalues_; ++i) {
        x1_.push_back(VertexDoubleValues(size, 0.0));
        x2_.push_back(VertexDoubleValues(size, 0.0));
        xsum_.push_back(VertexDoubleValues(size, 0.0));
        w1_.push_back(VertexDoubleValues(size, 1.0));
        w2_.push_back(VertexDoubleValues(size, 0.0));
        wsum_.push_back(VertexDoubleValues(size, 0.0));
      }
      size_t i = 0;
      for (auto p = values.cbegin(); p != values.cend(); ++p, ++i) {
        const VertexType v = *p;
        x1_[v][i] = 1.0;
      }
    }

    bool operator()(int step, unsigned int id, unsigned int nworkers) {
      const bool even = (step % 2) == 0;

      bool stop = true;
      for (VertexType col = 0; col < nvalues_; ++col) {

        VertexDoubleValues& x_prev = even ? x1_[col] : x2_[col];
        VertexDoubleValues& x_cur  = even ? x2_[col] : x1_[col];
        VertexDoubleValues& w_prev = even ? w1_[col] : w2_[col];
        VertexDoubleValues& w_cur  = even ? w2_[col] : w1_[col];
        VertexDoubleValues& x_sum  = xsum_[col];
        VertexDoubleValues& w_sum  = wsum_[col];

        for (size_t i = chunks_.ibegin(id), iend = chunks_.iend(id); i < iend; ++i) {
          const double y_prev = xsum_[col][i] / wsum_[col][i];
          x_cur[i] = 0.0;
          w_cur[i] = 0.0;

          EdgeList edges = vertices_[i].edges();
          for (auto q = edges.begin(), qend = edges.end(); q != qend; ++q) {
            VertexID j = q->dst();
            w_cur[i] += q->weight() * w_prev[j];
            x_cur[i] += q->weight() * x_prev[j];
          }

          x_sum[i] += (1-alpha_) * x_cur[i];
          w_sum[i] += (1-alpha_) * w_cur[i];
          w_cur[i] *= alpha_;
          x_cur[i] *= alpha_;
          stop &= std::abs(x_sum[i]/w_sum[i] - y_prev) < 1E-5;
        }
      }

      return stop;
    }

   /*
    bool operator()(int step, unsigned int id, unsigned int nworkers) {
      const bool even = (step % 2) == 0;
      RandomWalkResult& x_prev = even ? x1_ : x2_;
      RandomWalkResult& x_cur  = even ? x2_ : x1_;
      RandomWalkResult& w_prev = even ? w1_ : w2_;
      RandomWalkResult& w_cur  = even ? w2_ : w1_;

      VertexDoubleValues y_prev = VertexDoubleValues(nvalues_);

      bool stop = true;
      for (size_t i = chunks_.ibegin(id), iend = chunks_.iend(id); i < iend; ++i) {

        // Initialise y_prev x_cur and w_cur
        for (VertexType col = 0; col < nvalues_; ++col) {
          y_prev[col] = xsum_[col][i] / wsum_[col][i];
          x_cur[col][i] = 0.0;
          w_cur[col][i] = 0.0;
        }

        EdgeList edges = vertices_[i].edges();
        for (auto q = edges.begin(), qend = edges.end(); q != qend; ++q) {
          VertexID j = q->dst();

          for (VertexType col = 0; col < nvalues_; ++col) {
            w_cur[col][i] += q->weight() * w_prev[col][j];
            x_cur[col][i] += q->weight() * x_prev[col][j];
          }
        }

        for (VertexType col = 0; col < nvalues_; ++col) {
          xsum_[col][i] += (1-alpha_) * x_cur[col][i];
          wsum_[col][i] += (1-alpha_) * w_cur[col][i];
          w_cur[col][i] *= alpha_;
          x_cur[col][i] *= alpha_;
          stop &= std::abs(xsum_[col][i]/wsum_[col][i] - y_prev[col]) < 1E-5;
        }
      }

      return stop;
    }
    */

    /*bool operator()(int step, unsigned int id, unsigned int nworkers) {
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
    }*/

    RandomWalkResult result() const {
      RandomWalkResult res = xsum_;
      for (size_t i = 0, iend = vertices_.size(); i < iend; ++i) 
        for (VertexType col = 0; col < nvalues_; ++col) 
          res[col][i] /= wsum_[col][i];
      return res;
    }

    /*RandomWalkResult result() const {
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
    }*/

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
    const VertexList& vertices_;
    Chunker<VertexList> chunks_;
    VertexType nvalues_;
    double alpha_;
    RandomWalkResult xsum_, x1_, x2_;
    RandomWalkResult wsum_, w1_, w2_;
};



RandomWalkResult random_walk_categorical(const Graph& graph, 
    const VertexCategoricalValues& vertex_values, double alpha, 
    unsigned int nworkers, unsigned int nstep_max) {
  if (nworkers == 0) nworkers = determine_nthreads(graph.vertices().size());
  RandomWalkCategoricalComputation computation(nworkers, graph.vertices(), vertex_values, alpha);
  Stepper<RandomWalkCategoricalComputation> stepper(computation);
  stepper.run(nworkers, nstep_max);
  return computation.result();
}

