#include "random_walk.h"
#include <unordered_map>
#include <iostream>

class RWDataCat {
  public:
    RWDataCat(VertexType nvalues, VertexType val)  {
      n = nvalues + 1;
      total_score = new double[n] ();
      score = new double[2*n] ();
      cur = score;
      nxt = score + n;
      cur[val] = 1.0;
    }

    ~RWDataCat() {
      delete [] total_score;
      delete [] score;
    }

    void start_update() {
      for (VertexType i = 0; i < n; ++i) nxt[i] = 0.0;
    }

    bool end_update() {
      std::swap(cur, nxt);
      return true;
    }

    void update(const RWDataCat& data, double edge_weight, double alpha) {
      for (VertexType i = 0; i < n; ++i) {
        const double s  = data.cur[i] * edge_weight;
        nxt[i] += s * alpha;
        total_score[i] += s * (1-alpha);
      }
    }

    VertexType n;
    double* cur;
    double* nxt;
    double* total_score;
    double* score;
};

typedef std::unordered_map<VertexID, RWDataCat> RWDataValuesCat;


inline void print_values(const RWDataValuesCat& values) { 
  //for (auto p = values.begin(); p != values.end(); ++p) {
    //std::cout << p->first << ": score=" << p->second.total_score << 
      //"; weight=" << p->second.total_weight << 
      //"; cur score=[" << p->second.score[0] << "," << p->second.score[1] << "]" <<
      //"; cur weight=[" << p->second.weight[0] << "," << p->second.score[1] << "]" <<
      //"\n";
  //}
}

VertexType get_max_type(const VertexCategoricalValues& values) {
  VertexType max = 0;
  for (auto p = values.begin(); p != values.end(); ++p) {
    if (p->second > max) max = p->second;
  }
  return max;
}


void random_walk_cat(const Graph& graph, const VertexDoubleValues& vertex_values, double alpha) { 
  RWDataValuesCat values;

  // Initialise values
  VertexType nvalues = get_max_type(vertex_values);
  for (auto p = vertex_values.begin(); p != vertex_values.end(); ++p) {
    values[p->first] = RWDataCat(nvalues, p->second);
  }
  // Initialise loop
  unsigned int max_step = 50;
  const VertexList& vertices = graph.vertices();
  bool cont = true;
  // Start iterating
  for (unsigned int step = 0; step < max_step && cont; ++step) {
    // Loop over all edges
    for (auto vert = vertices.cbegin(); vert != vertices.cend(); ++vert) {
      RWDataCat& value = values[vert->second.id()];
      value.start_update();
      const EdgeList& edges = vert->second.edges();
      for (auto e = edges.cbegin(); e != edges.cend(); ++e) {
        const RWDataCat& target_value = values[e->dst()];
        value.update(target_value, e->weight(), alpha);
      }
    }
    // Signal next run and check if we need to stop
    cont = false;
    for (auto p = values.begin(); p != values.end(); ++p) cont |= p->second.end_update();
  }
  print_values(values);
}




