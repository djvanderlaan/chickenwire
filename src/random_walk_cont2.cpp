#include "random_walk.h"
#include <unordered_map>

class RWDataCont2 {
  public:
    RWDataCont2(double val = 0.0) : total_score(0.0), total_weight(0.0), 
      score{val, 0.0}, weight{1.0, 0.0} {
    }

    void start_update(int cur, int nxt) {
      score[nxt] = 0.0;
      weight[nxt] = 0.0;
    }

    bool end_update(int cur, int nxt) {
      return true;
    }

    void update(const RWDataCont2& data, int cur, int nxt, double edge_weight, double alpha) {
        const double s  = data.score[cur] * edge_weight;
        const double w = data.weight[cur] * edge_weight;
        score[nxt]   += s * alpha;
        weight[nxt]  += w * alpha;
        total_score  += s * (1-alpha);
        total_weight += w * (1-alpha);
    }

    double total_score;
    double total_weight;
    double score[2];
    double weight[2];
};

typedef std::unordered_map<VertexID, RWDataCont2> RWDataValuesCont2;


#include <iostream>
inline void print_values2(const RWDataValuesCont2& values) { 
  for (auto p = values.begin(); p != values.end(); ++p) {
    std::cout << p->first << ": score=" << p->second.total_score << 
      "; weight=" << p->second.total_weight << 
      "; cur score=[" << p->second.score[0] << "," << p->second.score[1] << "]" <<
      "; cur weight=[" << p->second.weight[0] << "," << p->second.score[1] << "]" <<
      "\n";
  }
}


void random_walk_cont2(const Graph& graph, const VertexDoubleValues& vertex_values, double alpha) { 
  RWDataValuesCont2 values;

  // Initialise values
  for (auto p = vertex_values.begin(); p != vertex_values.end(); ++p) {
    values[p->first] = RWDataCont2(p->second);
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

      RWDataCont2& value = values[vert->second.id()];
      value.start_update(cur, nxt);

      const EdgeList& edges = vert->second.edges();

      for (auto e = edges.cbegin(); e != edges.cend(); ++e) {
        const RWDataCont2& target_value = values[e->dst()];
        value.update(target_value, cur, nxt, alpha, e->weight());
      }
      cont |= value.end_update(cur, nxt);
      //if (prob > eps) cont = true; TODO
      //?? if ((1-total_weight) < eps) ??
      //>> if (value.weight[nxt] > eps) cont = true; ??
    }
    std::swap(nxt, cur);

    std::cout << "======================== " << step << " ===========================\n";
    print_values2(values);
    if (!cont) break;
  }
}



