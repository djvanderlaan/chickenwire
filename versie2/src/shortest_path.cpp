#include "shortest_path.h"

#include <unordered_map>
#include <queue>
#include <limits>

//#include <thread>
//#include <mutex>
//#include <functional>
//#include <cmath>

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

// Calculate the length of the shortest path between two vertices
template<typename T>
void dijkstra(const Graph& graph, VertexID from, T& search) {
  std::unordered_map<VertexID, double> path_lengths;
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
    // Process next element
    if (search.next(i.vertex, i.path_length)) break;
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
  search.finish(path_lengths, path_edges);
}


class ShortestPathSearch {
  public:
    ShortestPathSearch(VertexID to) : to_(to) {}

    bool next(VertexID id, double path_length) {
      return to == id;
    }
    
  private:
    VertexID to_;
    double result;
};

double shortest_path_length2(const Graph& graph, VertexID from, VertexID to) {
}



// ============================================================================
// === Shortest Path Length                                                 ===
// ============================================================================

// Calculate the length of the shortest path between two vertices
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
// === All Shortest Path Lengths                                            ===
// ============================================================================

// Calculate the shortest path between a vertex and all other nodes
PathLengths all_shortest_path_lengths(const Graph& graph, VertexID from) {
  std::unordered_map<VertexID, double>  path_lengths;
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

  // Convert unordered_map to vector
  PathLengths res(graph.vertices().size());
  for (auto p = path_lengths.cbegin(); p != path_lengths.cend(); ++p) 
    res[p->first] = p->second;
  return res;
}

// ============================================================================
// === All Shortest Paths                                                   ===
// ============================================================================

// Calculate the shortest path between a vertex and all other nodes
Graph all_shortest_paths(const Graph& graph, VertexID from) {
  std::unordered_map<VertexID, double> path_lengths;
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

