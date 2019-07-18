#include "shortest_path.h"

#include <unordered_map>
#include <queue>
#include <limits>

#include <thread>
#include <mutex>
#include <functional>
#include <cmath>

class QueueEl {
  public: 
    double path_length;
    VertexID vertex;
};

bool operator<(const QueueEl& a, const QueueEl& b) {
  return a.path_length > b.path_length;
}


// ============================================================================
// === General dijkstra algorithm                                           ===
// ============================================================================

template<typename T>
void dijkstra(const Graph& graph, VertexID from, T& search) {
  std::unordered_map<VertexID, QueueEl> path_lengths;
  std::priority_queue<QueueEl> queue;

  // Insert first node into the queue
  path_lengths[from] = QueueEl{0.0, from};
  queue.push(QueueEl{0.0, from});

  // Keep popping nodes from the queue and follow out edged of that node
  while (!queue.empty()) {
    const QueueEl& i = queue.top();
    const double path_length = i.path_length;
    const VertexID vertex_id = i.vertex;
    queue.pop();
    // Process next element
    if (search.next(i.vertex, i.path_length)) break;
    // Add subvertices of current vertex
    const Vertex& v = graph.vertex(vertex_id);
    const EdgeList& edges = v.edges();
    for (auto j = edges.cbegin(); j != edges.cend(); ++j) {
      auto f = path_lengths.find(j->dst());
      double newl = path_length + j->weight();
      if (f == path_lengths.end() || f->second.path_length > newl) {
        path_lengths[j->dst()] = QueueEl{newl, vertex_id};
        queue.push(QueueEl{newl, j->dst()});
      }
    }
  }
  search.finish(path_lengths);
}


// ============================================================================
// === Shortest Path Length                                                 ===
// ============================================================================

class ShortestPathLengthSearch {
  public:
    ShortestPathLengthSearch(VertexID to) : to_(to) {}

    bool next(VertexID id, double path_length) {
      return to_ == id;
    }

    void finish(const std::unordered_map<VertexID, QueueEl>& path_lengths) {
      auto p = path_lengths.find(to_);
      result_ = (p == path_lengths.end()) ? 
        std::numeric_limits<double>::infinity() : 
        p->second.path_length;
    }

    double result() const { return result_; }
    
  private:
    VertexID to_;
    double result_;
};

double shortest_path_length(const Graph& graph, VertexID from, VertexID to) {
  ShortestPathLengthSearch search(to);
  dijkstra(graph, from, search);
  return search.result();
}


// ============================================================================
// === Shortest Path                                                        ===
// ============================================================================

class ShortestPathSearch {
  public:
    ShortestPathSearch(VertexID from, VertexID to) : from_(from), to_(to) {}

    bool next(VertexID id, double path_length) {
      return to_ == id;
    }

    void finish(const std::unordered_map<VertexID, QueueEl>& path_lengths) {
      auto p = path_lengths.find(to_);
      if (p != path_lengths.end()) { 
        VertexID id = p->first;
        path_.push_back(PathElement{id, p->second.path_length});
        while (id != from_) {
          id = p->second.vertex;
          p = path_lengths.find(id);
          path_.push_back(PathElement{id, p->second.path_length});
        }
      } else {
        path_.push_back(PathElement{to_, std::numeric_limits<double>::infinity()});
      }
    }

    Path result() const { return path_; }
    
  private:
    VertexID from_;
    VertexID to_;
    Path path_;
};

Path shortest_path(const Graph& graph, VertexID from, VertexID to) {
  ShortestPathSearch search(from, to);
  dijkstra(graph, from, search);
  return search.result();
}



// ============================================================================
// === All Shortest Path Lengths                                            ===
// ============================================================================

class AllShortestPathLengthsSearch {
  public:
    AllShortestPathLengthsSearch(size_t size) : result_(size) {}

    bool next(VertexID id, double path_length) { return false; }

    void finish(const std::unordered_map<VertexID, QueueEl>& path_lengths) {
      for (auto p = path_lengths.cbegin(); p != path_lengths.cend(); ++p) 
        result_[p->first] = p->second.path_length;
    }

    PathLengths result() const { return result_; }
    
  private:
    PathLengths result_;
};

PathLengths all_shortest_path_lengths(const Graph& graph, VertexID from) {
  AllShortestPathLengthsSearch search(graph.vertices().size());
  dijkstra(graph, from, search);
  return search.result();
}



// ============================================================================
// === All Shortest Paths                                                   ===
// ============================================================================

class AllShortestPathsSearch {
  public:
    AllShortestPathsSearch(VertexID from) : from_(from), tree_(true) {}

    bool next(VertexID id, double path_length) { return false;}

    void finish(const std::unordered_map<VertexID, QueueEl>& path_lengths) {
      for (auto p = path_lengths.cbegin(), end = path_lengths.cend(); p != end; ++p) {
        tree_.add_vertex_if_not_exists(p->first);
        tree_.add_vertex_if_not_exists(p->second.vertex);
        tree_.add_edge(p->first, p->second.vertex, p->second.path_length);
      }
    }

    Graph result() const { return tree_; }
    
  private:
    VertexID from_;
    Graph tree_;
};

Graph all_shortest_paths(const Graph& graph, VertexID from) {
  AllShortestPathsSearch search(from);
  dijkstra(graph, from, search);
  return search.result();
}

// ============================================================================
// === Max Shortest Path                                                    ===
// ============================================================================

class MaxShortestPathLengthSearch {
  public:
    MaxShortestPathLengthSearch() : max_(0.0) {}

    bool next(VertexID id, double path_length) { return false;}

    void finish(const std::unordered_map<VertexID, QueueEl>& path_lengths) {
      for (auto p = path_lengths.cbegin(); p != path_lengths.cend(); ++p) {
        if (p->second.path_length > max_) max_ = p->second.path_length;
      }
    }

    double result() const { return max_; }
    
  private:
    double max_;
};

// Calculate the shortest path between a vertex and all other nodes; determine
// the maximum value of that.
double max_shortest_path(const Graph& graph, VertexID from) {
  MaxShortestPathLengthSearch search;
  dijkstra(graph, from, search);
  return search.result();
}

// ============================================================================
// === Diameter                                                             ===
// ============================================================================
// The diameter of a graph is the largest shortest path between any two nodes
// in the graph


void diameter_thread_fun(const Graph& graph, std::size_t begin, 
    std::size_t end, int& result, std::mutex& mutex) {
  const VertexList& v = graph.vertices();
  int diameter = 0;
  // forwarding to the place we have to start
  auto p = v.begin() + begin;
  // calculate maximum diameter
  for (size_t i = begin; i < end; ++p, ++i) {
    int d = max_shortest_path(graph, i);
    if (d > diameter) diameter = d;
  }
  // update global diameter
  std::lock_guard<std::mutex> guard(mutex);
  if (diameter > result) result = diameter;
}

double diameter(const Graph& graph, unsigned int nthreads) {
  if (nthreads == 0) throw std::runtime_error("Number of threads needs to be > 0.");
  std::vector<std::thread> threads;
  int diameter = 0;
  std::mutex diameter_mutex;
  std::size_t start = 0;
  for (unsigned int i = 0; i < nthreads; ++i) {
    std::size_t size = std::round((double)(graph.vertices().size() - start)/(nthreads-i));
    threads.push_back(std::thread(diameter_thread_fun, std::ref(graph), 
      start, start+size, std::ref(diameter), std::ref(diameter_mutex)));
    start  += size;
  }
  for (unsigned int i = 0; i < nthreads; ++i) threads[i].join();
  return diameter;
}

double diameter(const Graph& graph) {
  unsigned int nthreads = std::min(std::thread::hardware_concurrency(), 
    std::max((unsigned int)graph.vertices().size()/512, 1U));
  return diameter(graph, nthreads);
}

