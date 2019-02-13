
#include "random_walk.h"

#include <unordered_map>
#include <map>

#include <iostream>

void random_walk(const Graph& graph, vertex_id from, double alpha) {
  std::unordered_map<vertex_id, double> weights;

  // initialise queues
  typedef std::map<vertex_id, double> Queue;
  Queue queue0;
  Queue queue1;
  Queue& current_queue = queue0;
  Queue& next_queue = queue1;
  current_queue[from] = 1.0;

  unsigned int max_step = 25;
  double eps = 1E-5;

  for (unsigned int i = 0; i < max_step; ++i) {

    for (auto vid = current_queue.begin(); vid != current_queue.end(); ++vid) {
      
      const Vertex& vertex = graph.vertex(vid->first);
      const EdgeList& edges = vertex.edges_out();
      double prob = vid->second;

      for (auto e = edges.cbegin(); e != edges.cend(); ++e) {
        double p = prob * e->weight();
        weights[e->dst()] = weights[e->dst()] + p*(1.0-alpha);
        if (p > eps)
          next_queue[e->dst()] = next_queue[e->dst()] + p * alpha;
      }
    }
    current_queue.clear();
    std::swap(current_queue, next_queue);

    //std::cout << "STEP " << i << " ================================================\n";
    //for (auto p = weights.cbegin(); p != weights.cend(); ++p) {
      //std::cout << p->first << '\t' << p->second << '\n';
    //}
  }

  /*
  double max = 0.0;
  double sum = 0.0;
  for (auto p = weights.cbegin(); p != weights.cend(); ++p) {
    std::cout << p->first << '\t' << p->second << '\n';
    if (p->second > max) max = p->second;
    sum += p->second;
  }
  std::cout << "MAX=" << max << "\n";
  std::cout << "SUM=" << sum << "\n";
  */
}


void random_walk2(const Graph& graph, vertex_id from, double alpha) {
  unsigned int size = graph.vertices().size();

  double* weights = new double[size] ();

  // initialise queues
  double* probs0 = new double[size] ();
  double* probs1 = new double[size] ();
  double* cur = probs0;
  double* nxt = probs1;
  cur[from] = 1.0;

  unsigned int max_step = 25;
  double eps = 1E-5;

  for (unsigned int i = 0; i < max_step; ++i) {
    
    double* p = cur;

    for (std::size_t j = 0; j < size; ++j, ++p) {

      if (*p < eps) continue;

      const Vertex& vertex = graph.vertex(j);
      const EdgeList& edges = vertex.edges_out();

      for (auto e = edges.cbegin(); e != edges.cend(); ++e) {
        double prob = (*p) * e->weight();
        weights[e->dst()] = weights[e->dst()] + prob*(1.0-alpha);
        if (prob > eps)
          nxt[e->dst()] = nxt[e->dst()] + prob * alpha;
      }
      *p = 0;
    }
    std::swap(cur, nxt);

    //std::cout << "STEP " << i << " ================================================\n";
    //for (auto p = weights.cbegin(); p != weights.cend(); ++p) {
      //std::cout << p->first << '\t' << p->second << '\n';
    //}
  }

  /*double max = 0.0;
  double sum = 0.0;
  for (std::size_t i = 0; i < size; ++ i) {
    std::cout << i << '\t' << weights[i] << '\n';
    if (weights[i] > max) max = weights[i];
    sum += weights[i];
  }
  std::cout << "MAX=" << max << "\n";
  std::cout << "SUM=" << sum << "\n";
  */
  

  delete [] weights;
  delete [] probs0;
  delete [] probs1;

}

