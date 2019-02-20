
#include "random_walk.h"
#include <unordered_map>

class RWDataCont {
  public:
    RWDataCont(double val = 0.0) : total_score(0.0), total_weight(0.0), 
      score{val, 0.0}, weight{1.0, 0.0} {
    }

    double total_score;
    double total_weight;
    double score[2];
    double weight[2];
};

typedef std::unordered_map<VertexID, RWDataCont> RWDataValuesCont;


#include <iostream>
inline void print_values(const RWDataValuesCont& values) { 
  for (auto p = values.begin(); p != values.end(); ++p) {
    std::cout << p->first << ": score=" << p->second.total_score << 
      "; weight=" << p->second.total_weight << 
      "; cur score=[" << p->second.score[0] << "," << p->second.score[1] << "]" <<
      "; cur weight=[" << p->second.weight[0] << "," << p->second.score[1] << "]" <<
      "\n";
  }
}


void random_walk_cont(const Graph& graph, const VertexDoubleValues& vertex_values, double alpha) { 
  RWDataValuesCont values;

  // Initialise values
  for (auto p = vertex_values.begin(); p != vertex_values.end(); ++p) {
    values[p->first] = RWDataCont(p->second);
  }

  // Loop termination settings
  unsigned int max_step = 50;
  double eps = 1E-5;

  const VertexList& vertices = graph.vertices();

  int cur = 0;
  int nxt = 1;

  for (unsigned int step = 0; step < max_step; ++step) {

    bool cont = false;
    
    for (auto vert = vertices.cbegin(); vert != vertices.cend(); ++vert) {

      RWDataCont& value = values[vert->second.id()];
      value.score[nxt]  = 0.0;
      value.weight[nxt] = 0.0;

      const EdgeList& edges = vert->second.edges();

      for (auto e = edges.cbegin(); e != edges.cend(); ++e) {
        const double score  = values[e->dst()].score[cur] * e->weight();
        const double weight = values[e->dst()].weight[cur] * e->weight();

        value.score[nxt]   += score  * alpha;
        value.weight[nxt]  += weight * alpha;
        value.total_score  += score * (1-alpha);
        value.total_weight += weight * (1-alpha);
      }
      //if (prob > eps) cont = true; TODO
      //?? if ((1-total_weight) < eps) ??
      //>> if (value.weight[nxt] > eps) cont = true; ??
    }
    std::swap(nxt, cur);

    std::cout << "======================== " << step << " ===========================\n";
    print_values(values);
    //if (!cont) break;
  }
}


