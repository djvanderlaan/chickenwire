#include <iostream>
#include <iomanip>
#include <vector>
#include <exception>
#include <random>
#include <chrono>

class Graph {
  public:
    Graph(size_t nvertices) : nvertices_(nvertices), vertices_(new int*[nvertices]), edges_(new int[nvertices]), 
      nedges_max_(nvertices) {
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
    size_t nedges_max_;

    int** vertices_ = 0;
    int*  edges_ = 0;
    int current_vertex_for_adding_edges_ = -1;
};


/*
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
*/


int main(int argc, char* argv[]) {
  int n = 1E6;
  int k = 200;
  std::cout << argc << std::endl;
  if (argc > 1) n = std::atoi(argv[1]);
  if (argc > 2) k = std::atoi(argv[2]);
  std::cout << "n = " << n << std::endl;
  std::cout << "k = " << k << std::endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> rint(0, n-1);
  std::uniform_int_distribution<> rdegree(0, k);

  Graph graph(n);

  /*for (int i = 0; i < n; ++i) {
    int k = rdegree(gen);
    for (int j = 0; j < k; ++j) {
      int dst = rint(gen);
      graph.add_edge(i, dst, 1.0);
    }
  }

  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  normalise_weights(graph);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  std::cout << "Normalisation took " << dif/1000.0 << " seconds." << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  std::vector<int> in_degree = calc_in_degree(graph);
  t2 = std::chrono::high_resolution_clock::now();
  dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  std::cout << "calc_in_degree took " << dif/1000.0 << " seconds." << std::endl;


  t1 = std::chrono::high_resolution_clock::now();
  std::vector<int> components = connected_components(graph);
  t2 = std::chrono::high_resolution_clock::now();
  dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  std::cout << "connected components took " << dif/1000.0 << " seconds." << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  std::vector<int> components2 = connected_components3(graph);
  t2 = std::chrono::high_resolution_clock::now();
  dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  std::cout << "connected components took " << dif/1000.0 << " seconds." << std::endl;

  //for (int i = 0; i < graph.nvertices_; ++i) {
  for (int i = 0; i < 10; ++i) {
    std::cout << i << ": " << components[i] << "\n";
  }
  */
 

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

