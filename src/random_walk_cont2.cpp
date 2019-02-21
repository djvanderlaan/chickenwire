#include "random_walk.h"
#include <iostream>

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
      return true;
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



/*
typedef std::unordered_map<VertexID, RWDataCont2> RWDataValuesCont2;


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
  // Initialise loop
  unsigned int max_step = 50;
  const VertexList& vertices = graph.vertices();
  bool cont = true;
  // Start iterating
  for (unsigned int step = 0; step < max_step && cont; ++step) {
    // Loop over all edges
    for (auto vert = vertices.cbegin(); vert != vertices.cend(); ++vert) {
      RWDataCont2& value = values[vert->second.id()];
      value.start_update();
      const EdgeList& edges = vert->second.edges();
      for (auto e = edges.cbegin(); e != edges.cend(); ++e) {
        const RWDataCont2& target_value = values[e->dst()];
        value.update(target_value, e->weight(), alpha);
      }
    }
    // Signal next run and check if we need to stop
    cont = false;
    for (auto p = values.begin(); p != values.end(); ++p) cont |= p->second.end_update();
  }
  //std::cout << "RANDOM_WALK_CONT\n";
  //print_values2(values);
  //std::cout << "===========================================================\n";
}


*/
