
#include "random_walk.h"

#include <unordered_map>
#include <unordered_set>

void random_walk(const Graph& graph, vertex_id from, double alpha) {
  std::unordered_map<vertex_id, double> weights;

  // initialise queues
  std::unordered_set<vertex_id> queue0;
  std::unordered_set<vertex_id> queue1;
  std::unordered_set<vertex_id>& current_queue = queue0;
  std::unordered_set<vertex_id>& next_queue = queue1;
  current_queue.insert(from);

  unsigned int max_step = 10;

  for (unsigned int i = 0; i < max_step; ++i) {

    for (auto vid = current_queue.begin(); vid != current_queue.end(); ++vid) {
      
      const Vertex& vertex = graph.vertex(*vid);
      const EdgeList& edges = vertex.edges_out();

      for (auto e = edges.cbegin(); e != edges.cend(); ++e) {
        weights[e->dst()] = weights[e->dst()] + e->weight(); // TODO
        next_queue.insert(e->dst());
      }
    }
    current_queue.clear();
    std::swap(current_queue, next_queue);

  }
}

