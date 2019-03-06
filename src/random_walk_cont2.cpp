#include "random_walk.h"
#include <iostream>

#include<thread>
//#include<mutex>
//#include<functional>
#include<vector>

class RWDataCont2 {
  public:
    RWDataCont2(double val = 0.0) : cur(0), total_score(0.0), total_weight(0.0), 
      score{val, 0.0}, weight{1.0, 0.0} {
    }

    void start_update() {
      score[!cur] = 0.0;
      weight[!cur] = 0.0;
    }

    bool end_update() {
      cur = !cur;
      return (weight[cur] > 1E-4);
    }

    void update(const RWDataCont2& data, double edge_weight, double alpha) {
      const double s  = data.score[cur] * edge_weight;
      const double w = data.weight[cur] * edge_weight;
      score[!cur]   += s * alpha;
      weight[!cur]  += w * alpha;
      total_score  += s * (1-alpha);
      total_weight += w * (1-alpha);
    }

    unsigned char cur;
    double total_score;
    double total_weight;
    double score[2];
    double weight[2];
};

void random_walk_cont2(const Graph& graph, const VertexDoubleValues& vertex_values, double alpha) { 
  return random_walk_template<RWDataCont2>(graph, vertex_values, alpha);
}



// ============================================================================
// ==== PARALLEL
// ============================================================================

class RWThreadFun {
  public:
    typedef std::unordered_map<VertexID, RWDataCont2> Data;

    RWThreadFun(Data& data, VertexList::const_iterator start, VertexList::const_iterator end, 
        double alpha) : data_(data), start_(start), end_(end), alpha_(alpha), cont_(true) {
    }

    void update() {
      if (!cont_) return;
      for (auto vert = start_; vert != end_; ++vert) {
        RWDataCont2& value = data_[vert->second.id()];
        value.start_update();
        const EdgeList& edges = vert->second.edges();
        for (auto e = edges.cbegin(); e != edges.cend(); ++e) {
          const RWDataCont2& target_value = data_[e->dst()];
          value.update(target_value, e->weight(), alpha_);
        }
      }
    }

    void end_update() {
      cont_ = false;
      for (auto vert = start_; vert != end_; ++vert) {
        RWDataCont2& value = data_[vert->second.id()];
        cont_ |= value.end_update();
      }
    }

    bool cont() const { return cont_;}

  private:
    Data& data_;
    VertexList::const_iterator start_, end_;
    double alpha_;
    bool cont_;
};

#include <cmath>

void random_walk_cont_thread(const Graph& graph, const std::unordered_map<VertexID, double>& vertex_values, double alpha) { 
  // Initialise values
  std::unordered_map<VertexID, RWDataCont2> values = initialise_rw_data<RWDataCont2>(graph, vertex_values);
  // Set up threads
  unsigned int nthreads = 4U;
  std::vector<RWThreadFun> funs;
  std::size_t start = 0;
  const VertexList& vertices = graph.vertices();
  auto p = vertices.cbegin();
  for (unsigned int i = 0; i < nthreads; ++i) {
    std::size_t size = std::round((double)(graph.vertices().size() - start)/(nthreads-i));
    auto start_vertex = p;
    for (std::size_t j = 0; j < size; ++start, ++j) ++p;
    funs.push_back(RWThreadFun(values, start_vertex, p, alpha));
  }

  // Initialise loop
  unsigned int max_step = 150;
  RWThreadFun threadfun(values, vertices.cbegin(), vertices.cend(), alpha);
  bool cont = true;
  // Start iterating
  for (unsigned int step = 0; step < max_step && cont; ++step) {
    std::vector<std::thread> threads;
    for (auto p = funs.begin(); p != funs.end(); ++p) 
      threads.push_back(std::thread(&RWThreadFun::update, *p));
    for (auto p = threads.begin(); p != threads.end(); ++p) p->join();

    //for (auto p = funs.begin(); p != funs.end(); ++p) p->update();
    //threadfun.update();
    threads.clear();
    for (auto p = funs.begin(); p != funs.end(); ++p) 
      threads.push_back(std::thread(&RWThreadFun::end_update, *p));
    for (auto p = threads.begin(); p != threads.end(); ++p) p->join();

    //for (auto p = funs.begin(); p != funs.end(); ++p) p->end_update();
    cont = false;
    for (auto p = funs.begin(); p != funs.end(); ++p) cont |= p->cont();
    //cont = threadfun.end_update();
    if (!cont) std::cout << "Terminating iteration after step " << step << ".\n";
  }
}


