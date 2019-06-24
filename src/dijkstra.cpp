#include "functions.h"

#include<unordered_map>
#include<queue>
#include<limits>
#include<thread>
#include<mutex>
#include<functional>
#include<cmath>
#include<vector>

#include <iostream>
#include <iomanip>

class QueueEl {
  public: 
    double path_length;
    VertexID vertex;
};

bool operator<(const QueueEl& a, const QueueEl& b) {
  return a.path_length > b.path_length;
}

// ============================================================================
// === Shortest Path                                                        ===
// ============================================================================

// Calculate the shortest path between two vertices
double shortest_path(const Graph& graph, VertexID from, VertexID to) {
  std::unordered_map<VertexID, double> path_lengths;
  std::priority_queue<QueueEl> queue;

  // Insert first node into the queue
  path_lengths[from] = 0.0;
  queue.push(QueueEl{0.0, from});

  // Keep popping nodes from the queue and follow out edged of that node
  while (!queue.empty()) {
    const QueueEl& i = queue.top();
    // When we have found our target
    if (i.vertex == to) return path_lengths[to];
    // Add subvertices of current vertex
    const Vertex& v = graph.vertex(i.vertex);
    const EdgeList& edges = v.edges();
    for (auto j = edges.cbegin(); j != edges.cend(); ++j) {
      auto f = path_lengths.find(j->dst());
      double newl = i.path_length + j->weight();
      if (f == path_lengths.end() || f->second > newl) {
        path_lengths[j->dst()] = newl;
        queue.push(QueueEl{newl, j->dst()});
      }
    }
    queue.pop();
  }
  return std::numeric_limits<double>::infinity();
}


// ============================================================================
// === All Shortest Paths                                                   ===
// ============================================================================

// Calculate the shortest path between a vertex and all other nodes
PathLengths all_shortest_paths(const Graph& graph, VertexID from) {
  std::cout << "foo" << std::endl;
  PathLengths path_lengths;
  std::cout << "foo" << std::endl;
  std::priority_queue<QueueEl> queue;
  std::cout << "foo" << std::endl;

  // Insert first node into the queue
  path_lengths[from] = 0.0;
  std::cout << "foo" << std::endl;
  queue.push(QueueEl{0.0, from});
  std::cout << "foo" << std::endl;

  // Keep popping nodes from the queue and follow out edged of that node
  while (!queue.empty()) {
    std::cout << "bar" << std::endl;
    const QueueEl& i = queue.top();
    std::cout << "bar" << std::endl;
    // Add subvertices of current vertex
    const Vertex& v = graph.vertex(i.vertex);
    std::cout << "bar" << std::endl;
    const EdgeList& edges = v.edges();
    std::cout << "bar" << std::endl;
    for (auto j = edges.cbegin(); j != edges.cend(); ++j) {
      std::cout << "foobar" << std::endl;
      auto f = path_lengths.find(j->dst());
      std::cout << "foobar" << std::endl;
      //std::cout << i == queue.end() << std::endl;
      std::cout << i.path_length << std::endl;
      std::cout << j->weight() << std::endl;
      double newl = i.path_length + j->weight();
      std::cout << "foobar" << std::endl;
      if (f == path_lengths.end() || f->second > newl) {
      std::cout << "foobar" << std::endl;
        path_lengths[j->dst()] = newl;
      std::cout << "foobar" << std::endl;
        queue.push(QueueEl{newl, j->dst()});
      std::cout << "foobar" << std::endl;
      }
      std::cout << "foobarend" << std::endl;
    }
    std::cout << "bar" << std::endl;
    queue.pop();
    std::cout << "barend" << std::endl;
  }
  return path_lengths;
}



// ============================================================================
// === Max Shortest Path                                                    ===
// ============================================================================

// Calculate the shortest path between a vertex and all other nodes; determine
// the maximum value of that.
double max_shortest_path(const Graph& graph, VertexID from) {
  std::unordered_map<VertexID, double> path_lengths;
  std::priority_queue<QueueEl> queue;

  // Insert first node into the queue
  path_lengths[from] = 0.0;
  queue.push(QueueEl{0.0, from});

  // Keep popping nodes from the queue and follow out edged of that node
  double max = 0.0;
  while (!queue.empty()) {
    const QueueEl& i = queue.top();
    if (i.path_length > max) max = i.path_length;
    // Add subvertices of current vertex
    const Vertex& v = graph.vertex(i.vertex);
    const EdgeList& edges = v.edges();
    for (auto j = edges.cbegin(); j != edges.cend(); ++j) {
      auto f = path_lengths.find(j->dst());
      double newl = i.path_length + j->weight();
      if (f == path_lengths.end() || f->second > newl) {
        path_lengths[j->dst()] = newl;
        queue.push(QueueEl{newl, j->dst()});
      }
    }
    queue.pop();
  }
  return max;
}


// ============================================================================
// === Diameter                                                             ===
// ============================================================================
// The diameter of a graph is the largest shortest path between any two nodes
// in the graph


void diameter_thread_fun(const Graph& graph, std::size_t begin, 
    std::size_t end, int& result, std::mutex& mutex) {
  auto v = graph.vertices();
  int diameter = 0;
  // forwarding to the place we have to start; unfortunately the iterator for a 
  // map doesn't support the + operator
  auto p = v.begin();
  std::size_t i = 0;
  for (; i < begin; ++i) ++p;
  // calculate maximum diameter
  for (; i < end; ++p, ++i) {
    int d = max_shortest_path(graph, p->first);
    if (d > diameter) diameter = d;
  }
  // update global diameter
  std::lock_guard<std::mutex> guard(mutex);
  if (diameter > result) result = diameter;
}

int diameter(const Graph& graph) {
  unsigned int nthreads = std::min(std::thread::hardware_concurrency(), 
    std::max((unsigned int)graph.vertices().size()/512, 1U));
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

