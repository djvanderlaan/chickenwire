#ifndef random_walk_h
#define random_walk_h

#include "graph.h"
#include <unordered_map>

// ======== INPUT TYPES
// A number of classes are defined that can be used to pass data to the random walk
// functions. These functions expect the graph and the values of the vertices.

typedef std::unordered_map<VertexID, double> VertexDoubleValues;
typedef std::unordered_map<VertexID, VertexType> VertexCategoricalValues;

// Weighted versions; these consist of a value and a weight
struct WDouble {
   double value;
   double weight;
};

struct WCategorical {
  VertexType value;
  double weight;
};

typedef std::unordered_map<VertexID, WDouble> VertexWDoubleValues;
typedef std::unordered_map<VertexID, WCategorical> VertexWCategoricalValues;

// ========= RESULT TYPE
#include <memory>
#include <cstring>

class RandomWalkResult {
  public: 
    RandomWalkResult(size_t size, VertexType nvalues) : size_(size), nvalues_(nvalues),
        data_(new double[size * nvalues]), cur_(0) {
      const size_t n = size_*nvalues_;
      for (size_t i = 0; i < n; ++i) data_[i] = 0.0;
    }

    RandomWalkResult(const RandomWalkResult& other): size_(other.size_), 
        nvalues_(other.nvalues_), map_(other.map_), data_(new double[size_ * nvalues_]), cur_(0) {
      std::memcpy(other.data_.get(), data_.get(), sizeof(double)*size_*nvalues_);
    }

    size_t size() const { return size_;}
    VertexID nvalues() const { return nvalues_;}

    double* insert(VertexID id) {
      auto p = map_.find(id);
      size_t pos = 0;
      if (p == map_.end()) {
        pos = (cur_++) * nvalues_;
        map_[id] = pos;
      } else {
        pos = p->second;
      }
      return data_.get() + pos;
    }

    void set(VertexID id, VertexType i, double value) {
      auto   p = map_.find(id);
      size_t pos = cur_;
      if (p == map_.end()) {
        pos = (cur_++) * nvalues_;
        map_[id] = pos;
      } else {
        pos = p->second;
      }
      data_[pos + i] = value;
    }

    double get(VertexID id, VertexType i) const {
      return data_[map_.at(id)+i];
    }

    double* getp(VertexID id) {
      return data_.get() + map_.at(id);
    }

    const double* getp(VertexID id) const {
      return data_.get() + map_.at(id);
    }
  private:
    const RandomWalkResult& operator=(const RandomWalkResult& other) { return *this;};
    size_t size_;
    VertexType nvalues_;
    std::unordered_map<VertexID, size_t> map_;
    std::unique_ptr<double[]> data_;
    size_t cur_;
};


// ========== RANDOM WALK FUNCTONS

RandomWalkResult random_walk_continuous(const Graph& graph, const VertexDoubleValues& vertex_values, 
  double alpha = 0.85, unsigned int nworkers = 0);
RandomWalkResult random_walk_continuous(const Graph& graph, const VertexWDoubleValues& vertex_values, 
  double alpha = 0.85, unsigned int nworkers = 0);

RandomWalkResult random_walk_categorical(const Graph& graph, const VertexCategoricalValues& vertex_values, 
  double alpha = 0.85, unsigned int nworkers = 0);
RandomWalkResult random_walk_categorical(const Graph& graph, const VertexWCategoricalValues& vertex_values, 
  double alpha = 0.85, unsigned int nworkers = 0);

#endif
