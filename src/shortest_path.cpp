#include "shortest_path.h"

#include<queue>
#include<limits>
#include<thread>
#include<mutex>
#include<functional>
#include<cmath>

class QueueEl {
  public: 
    double path_length;
    VertexID vertex;
};

bool operator<(const QueueEl& a, const QueueEl& b) {
  return a.path_length > b.path_length;
}

// ============================================================================
// === Shortest Path Length                                                 ===
// ============================================================================

// Calculate the shortest path between two vertices
double shortest_path_length(const Graph& graph, VertexID from, VertexID to) {
  std::unordered_map<VertexID, double> path_lengths;
  std::priority_queue<QueueEl> queue;

  // Insert first node into the queue
  path_lengths[from] = 0.0;
  queue.push(QueueEl{0.0, from});

  // Keep popping nodes from the queue and follow out edged of that node
  while (!queue.empty()) {
    const QueueEl& i = queue.top();
    const double path_length = i.path_length;
    const VertexID vertex_id = i.vertex;
    queue.pop();
    // When we have found our target
    if (vertex_id == to) return path_lengths[to];
    // Add subvertices of current vertex
    const Vertex& v = graph.vertex(vertex_id);
    const EdgeList& edges = v.edges();
    for (auto j = edges.cbegin(); j != edges.cend(); ++j) {
      auto f = path_lengths.find(j->dst());
      double newl = path_length + j->weight();
      if (f == path_lengths.end() || f->second > newl) {
        path_lengths[j->dst()] = newl;
        queue.push(QueueEl{newl, j->dst()});
      }
    }
  }
  return std::numeric_limits<double>::infinity();
}


// ============================================================================
// === Shortest Path                                                        ===
// ============================================================================

// Calculate the shortest path between two vertices
Path shortest_path(const Graph& graph, VertexID from, VertexID to) {
  std::unordered_map<VertexID, VertexID> path_edges;
  std::unordered_map<VertexID, double> path_lengths;
  std::priority_queue<QueueEl> queue;

  // Insert first node into the queue
  path_lengths[from] = 0.0;
  queue.push(QueueEl{0.0, from});

  // Keep popping nodes from the queue and follow out edged of that node
  while (!queue.empty()) {
    const QueueEl& i = queue.top();
    const double path_length = i.path_length;
    const VertexID vertex_id = i.vertex;
    queue.pop();
    // When we have found our target stop searching
    if (vertex_id == to) break;

    // Add subvertices of current vertex
    const Vertex& v = graph.vertex(vertex_id);
    const EdgeList& edges = v.edges();
    for (auto j = edges.cbegin(); j != edges.cend(); ++j) {
      auto f = path_lengths.find(j->dst());
      double newl = path_length + j->weight();
      if (f == path_lengths.end() || f->second > newl) {
        path_edges[j->dst()] = vertex_id;
        path_lengths[j->dst()] = newl;
        queue.push(QueueEl{newl, j->dst()});
      }
    }
  }

  // Construct shortest path
  Path path;
  if (path_lengths.find(to) != path_lengths.end()) { 
    VertexID id = to;
    path.push_back(PathElement{id, path_lengths[id]});
    while (id != from) {
      id = path_edges[id];
      path.push_back(PathElement{id, path_lengths[id]});
    }
  } else {
    path.push_back(PathElement{to, std::numeric_limits<double>::infinity()});
  }
  return path;
}

// ============================================================================
// === All Shortest Paths                                                   ===
// ============================================================================

// Calculate the shortest path between a vertex and all other nodes
Graph all_shortest_paths(const Graph& graph, VertexID from) {
  PathLengths path_lengths;
  std::unordered_map<VertexID, VertexID> path_edges;
  std::priority_queue<QueueEl> queue;

  // Insert first node into the queue
  path_lengths[from] = 0.0;
  queue.push(QueueEl{0.0, from});

  // Keep popping nodes from the queue and follow out edged of that node
  while (!queue.empty()) {
    const QueueEl& i = queue.top();
    const double path_length = i.path_length;
    const VertexID vertex_id = i.vertex;
    queue.pop();
    // Add subvertices of current vertex
    const Vertex& v = graph.vertex(vertex_id);
    const EdgeList& edges = v.edges();
    for (auto j = edges.cbegin(); j != edges.cend(); ++j) {
      auto f = path_lengths.find(j->dst());
      double newl = path_length + j->weight();
      if (f == path_lengths.end() || f->second > newl) {
        path_edges[j->dst()] = vertex_id;
        path_lengths[j->dst()] = newl;
        queue.push(QueueEl{newl, j->dst()});
      }
    }
  }

  // Construct graph with all paths
  Graph tree(true);
  // Start by adding all vertices [TODO] Do we want that; or only store those
  // vertices that have a path to `from`?
  // Add edges
  for (auto p = path_edges.cbegin(), end = path_edges.cend(); p != end; ++p) {
    tree.add_vertex_if_not_exists(p->first);
    tree.add_vertex_if_not_exists(p->second);
    tree.add_edge(p->first, p->second, path_lengths[p->first]);
  }
  return tree;;
}



// ============================================================================
// === All Shortest Paths                                                   ===
// ============================================================================

// Calculate the shortest path between a vertex and all other nodes
PathLengths all_shortest_path_lengths(const Graph& graph, VertexID from) {
  PathLengths path_lengths;
  std::priority_queue<QueueEl> queue;

  // Insert first node into the queue
  path_lengths[from] = 0.0;
  queue.push(QueueEl{0.0, from});

  // Keep popping nodes from the queue and follow out edged of that node
  while (!queue.empty()) {
    const QueueEl& i = queue.top();
    const double path_length = i.path_length;
    const VertexID vertex_id = i.vertex;
    queue.pop();
    // Add subvertices of current vertex
    const Vertex& v = graph.vertex(vertex_id);
    const EdgeList& edges = v.edges();
    for (auto j = edges.cbegin(); j != edges.cend(); ++j) {
      auto f = path_lengths.find(j->dst());
      double newl = path_length + j->weight();
      if (f == path_lengths.end() || f->second > newl) {
        path_lengths[j->dst()] = newl;
        queue.push(QueueEl{newl, j->dst()});
      }
    }
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
    const double path_length = i.path_length;
    const VertexID vertex_id = i.vertex;
    queue.pop();
    if (path_length > max) max = path_length;
    // Add subvertices of current vertex
    const Vertex& v = graph.vertex(vertex_id);
    const EdgeList& edges = v.edges();
    for (auto j = edges.cbegin(); j != edges.cend(); ++j) {
      auto f = path_lengths.find(j->dst());
      double newl = path_length + j->weight();
      if (f == path_lengths.end() || f->second > newl) {
        path_lengths[j->dst()] = newl;
        queue.push(QueueEl{newl, j->dst()});
      }
    }
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

