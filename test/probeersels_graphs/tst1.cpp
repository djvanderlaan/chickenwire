#include <iostream>
#include <iomanip>
#include <vector>
#include <exception>
#include <random>
#include <chrono>

//class Edge {
  //public:
    //Graph* graph;
    //int i;
    //int src_;
//
    //int src() {
      //return src;
    //}
    //int dst() {
      //return graph->out_edges_(i);
    //}
//};

class Graph {
  public:
    Graph(size_t nvertices) : nvertices_(nvertices), vertices_(nvertices, -1L), 
      out_degree_(nvertices, 0L) {
    }

    ~Graph() {
    }

    void add_edge(int src, int dst, float weight = 1.0) {
      if (src < current_vertex_for_adding_edges_) {
        throw std::runtime_error("Edges should be added in vertex order.");
      } else if (src > current_vertex_for_adding_edges_) {
        current_vertex_for_adding_edges_ = src;
        vertices_[src] = out_edges_.size();
      } 
      ++out_degree_[src];
      out_edges_.push_back(dst);
      out_weights_.push_back(weight);
    }


  public:
    size_t nvertices_;
    std::vector<int> out_edges_;       // size nedges
    std::vector<float> out_weights_;   // size nedges
    std::vector<int> vertices_;        // size nvertices
    std::vector<int> out_degree_;      // size nvertices
    int current_vertex_for_adding_edges_ = -1;
};

void normalise_weights(Graph& graph) {
  auto pw = graph.out_weights_.begin();
  for (int k: graph.out_degree_) {
    float total_weight = 0;
    auto q = pw;
    for (int i = 0; i < k; ++i, ++q) total_weight += *q;
    float scale = 1.0/total_weight;
    for (int i = 0; i < k; ++i, ++pw) *pw *= scale;
  }
}

std::vector<int> calc_in_degree(const Graph& graph) {
  std::vector<int> in_degree(graph.nvertices_, 0L);
  auto p = graph.out_edges_.begin();
  for (int k: graph.out_degree_) {
    for (int i = 0; i < k; ++i, ++p) ++in_degree[*p];
  }
  return in_degree;
}

std::vector<int> connected_components(const Graph& graph) {
  std::vector<int> components;
  components.reserve(graph.nvertices_);
  for (int i = 0; i < graph.nvertices_; ++i) components[i] = i;

  bool changed = true;
  int iteration = 0;
  while (changed) {
    changed = false;
    auto src = components.begin();
    auto p = graph.out_edges_.begin();
    for (int k: graph.out_degree_) {
      int component_src = *src;
      for (int i = 0; i < k; ++i, ++p) {
        int component_dst = components[*p];
        if (component_dst > component_src) {
          components[*p] = component_src;
          changed = true;
        } else if (component_dst < component_src){
          *src = component_dst;
          component_src = component_dst;
          changed = true;
        }
      }
      ++src;
    }
    ++iteration;
  }
  std::cout << "iterations = " << iteration << "\n";
  return components;
}

std::vector<int> connected_components2(const Graph& graph) {
  std::vector<int> components;
  components.reserve(graph.nvertices_);
  for (int i = 0; i < graph.nvertices_; ++i) components[i] = i;

  bool changed = true;
  int iteration = 0;
  while (changed) {
    changed = false;
    auto src = components.begin();
    auto p = graph.out_edges_.begin();
    for (int k: graph.out_degree_) {
      // determine component number
      int component = *src;
      auto q = p;
      for (int i = 0; i < k; ++i, ++q) {
        int component_dst = components[*q];
        component = component < component_dst ? component : component_dst;
      }
      if (component != *src) changed = true;
      // update component number
      for (int i = 0; i < k; ++i, ++p) components[*p] = component;
      *src = component;
      ++src;
    }
    ++iteration;
  }
  std::cout << "iterations = " << iteration << "\n";
  return components;
}

std::vector<int> connected_components3(const Graph& graph) {
  std::vector<int> components;
  components.reserve(graph.nvertices_);
  for (int i = 0; i < graph.nvertices_; ++i) components[i] = i;

  bool changed = true;
  while (changed) {

    int nedges = graph.out_edges_.size();
    auto src_degree  = graph.out_degree_.begin();
    auto edge = graph.out_edges_.begin();
    auto src_component = components.begin();
    int  edge_i = 0;
    changed = false;
    for (int dst: graph.out_edges_) {
      while (edge_i >= *src_degree) {
        edge_i = 0;
        ++src_component;
      }
      int dst_component = components[dst];
      if (dst_component > *src_component) {
        components[dst] = *src_component;
        changed = true;
      } else if (dst_component < *src_component) {
        *src_component = dst_component;
        changed = true;
      }
      ++edge_i;
    }
  }
  return components;
}


int main(int argc, char* argv[]) {
  int n = 2E6;
  int k = 200;
  if (argc > 1) n = std::atoi(argv[1]);
  if (argc > 2) k = std::atoi(argv[2]);
  std::cout << "n = " << n << std::endl;
  std::cout << "k = " << k << std::endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> rint(0, n-1);
  std::uniform_int_distribution<> rdegree(0, k);

  Graph graph(n);

  for (int i = 0; i < n; ++i) {
    int k = rdegree(gen);
    for (int j = 0; j < k; ++j) {
      int dst = rint(gen);
      graph.add_edge(i, dst, 1.0);
    }
  }

  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  //normalise_weights(graph);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  //std::cout << "Normalisation took " << dif/1000.0 << " seconds." << std::endl;

  //t1 = std::chrono::high_resolution_clock::now();
  //std::vector<int> in_degree = calc_in_degree(graph);
  //t2 = std::chrono::high_resolution_clock::now();
  //dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  //std::cout << "calc_in_degree took " << dif/1000.0 << " seconds." << std::endl;


  t1 = std::chrono::high_resolution_clock::now();
  std::vector<int> components = connected_components(graph);
  t2 = std::chrono::high_resolution_clock::now();
  dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  std::cout << "connected components took " << dif/1000.0 << " seconds." << std::endl;

  //t1 = std::chrono::high_resolution_clock::now();
  //std::vector<int> components2 = connected_components3(graph);
  //t2 = std::chrono::high_resolution_clock::now();
  //dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  //std::cout << "connected components took " << dif/1000.0 << " seconds." << std::endl;

  //for (int i = 0; i < graph.nvertices_; ++i) {
  for (int i = 0; i < 10; ++i) {
    std::cout << i << ": " << components[i] << "\n";
  }
 

  /*int src = 0;
  auto p = graph.out_edges_.begin();
  auto q = graph.out_weights_.begin();
  for (int k: graph.out_degree_) {
    std::cout << "out degree = " << k << std::endl;
    for (int i = 0; i < k; ++i, ++p, ++q) {
      std::cout << src << "->" << *p << " " << *q << "\n"; 
    }
    ++src;
  }*/

  return 0;
}

