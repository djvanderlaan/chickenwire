#include "random_walk.h"
#include <iostream>

class RWDataCat {
  public:
    RWDataCat(VertexType nvalues = 0, VertexType val = 0)  {
      n = nvalues + 1;
      total_score = new double[n] ();
      score = new double[2*n] ();
      cur = score;
      nxt = score + n;
      cur[val] = 1.0;
    }

    RWDataCat(const RWDataCat& data) : n(data.n) {
      if (total_score) delete [] total_score;
      if (score) delete [] score;
      total_score = new double[n] ();
      score = new double[2*n] ();
      if (data.cur == data.score) {
        cur = score;
        nxt = score + n;
      } else {
        cur = score + n;
        nxt = score;
      }
      for (int i = 0; i < n; ++i) {
        cur[i] = data.cur[i];
        nxt[i] = data.nxt[i];
        score[i] = data.score[i];
      }
    }

    RWDataCat& operator=(const RWDataCat& data) {
      if (n != data.n) {
        if (total_score) delete [] total_score;
        if (score) delete [] score;
        n = data.n;
        total_score = new double[n] ();
        score = new double[2*n] ();
      }
      if (data.cur == data.score) {
        cur = score;
        nxt = score + n;
      } else {
        cur = score + n;
        nxt = score;
      }
      for (int i = 0; i < n; ++i) {
        cur[i] = data.cur[i];
        nxt[i] = data.nxt[i];
        score[i] = data.score[i];
      }
      return *this;
    }

    ~RWDataCat() {
      if (total_score) delete [] total_score;
      if (score) delete [] score;
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


VertexType get_max_type(const VertexCategoricalValues& values) {
  VertexType max = 0;
  for (auto p = values.begin(); p != values.end(); ++p) {
    if (p->second > max) max = p->second;
  }
  return max;
}

template<typename S>
std::unordered_map<VertexID, RWDataCat> initialise_rw_data(const Graph& graph, const S& vertex_values) {
  VertexType nvalues = get_max_type(vertex_values);
  std::unordered_map<VertexID, RWDataCat> values;
  for (auto p = vertex_values.begin(); p != vertex_values.end(); ++p) {
    values[p->first] = RWDataCat(nvalues, p->second);
  }
  return values;
}

void random_walk_cat(const Graph& graph, const VertexCategoricalValues& vertex_values, double alpha) { 
  return random_walk_template<RWDataCat>(graph, vertex_values, alpha);
}

/*
//inline void print_values(const RWDataValuesCat& values) { 
//  for (auto p = values.begin(); p != values.end(); ++p) {
//    std::cout << p->first << ": score=";
//    for (VertexType i = 0; i < p->second.n; ++i) {
//      std::cout << i << ": " << p->second.total_score[i] << " ";
//    }
//    std::cout << "\n";
//  }
//}


typedef std::unordered_map<VertexID, RWDataCat> RWDataValuesCat;
void random_walk_cat(const Graph& graph, const VertexCategoricalValues& vertex_values, double alpha) { 

  // Initialise values
  RWDataValuesCat values = initialise_rw_data<RWDataCat>(graph, vertex_values);

  //RWDataValuesCat values;
  //VertexType nvalues = get_max_type(vertex_values);
  //for (auto p = vertex_values.begin(); p != vertex_values.end(); ++p) {
    //values[p->first] = RWDataCat(nvalues, p->second);
  //}
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
  //std::cout << "RANDOM_WALK_CAT\n";
  //print_values(values);
  //std::cout << "===========================================================\n";
}



*/

