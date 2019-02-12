#include "graph.h"
#include "functions.h"
#include "filter_edges.h"
#include "random_walk.h"
#include "random_graph.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <map>
#include <cstdlib>
#include <unordered_set>

Graph generate_graph(int size) {
  std::cout << "Generating graph" << std::endl;
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  Graph graph(false);
  barabasi_albert(graph, size);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
  std::cout << "Took " << dif << " seconds." << std::endl;
  return graph;
}

void dump_to_file(Graph& graph, const std::string& filename) {
  std::cout << "Dumping to file '" << filename << "'" << std::endl;
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  graph_to_csv(graph, filename + "_vertices.csv", filename + "_edges.csv"); 
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
  std::cout << "Took " << dif << " seconds." << std::endl;
}

Graph read_from_file(const std::string& filename) {
  std::cout << "Reading from file '" << filename << "'" << std::endl;
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  Graph graph = graph_from_csv(filename + "_vertices.csv", filename + "_edges.csv"); 
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
  std::cout << "Took " << dif << " seconds." << std::endl;
  return graph;
}

unsigned int ncomponents(const Graph& graph) {
  std::cout << "Computing connected components" << std::endl;
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  Components components = connected_components(graph);
  std::unordered_set<vertex_id> unique_components;
  for (auto p = components.cbegin(); p != components.cend(); ++p) {
    unique_components.insert(p->second);
  }
  std::cout << "Number of components: " << unique_components.size() << std::endl;
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
  std::cout << "Took " << dif << " seconds." << std::endl;
  return unique_components.size();
}

int calc_diameter(const Graph& graph) {
  std::cout << "Computing diameter" << std::endl;
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  int diam = diameter(graph);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  std::cout << "Diameter = " << diam << std::endl;
  double dif = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
  std::cout << "Took " << dif << " seconds." << std::endl;
  return diam;
}

void print_graph(const Graph& graph, std::ostream& out) {
  const VertexList& v = graph.vertices();
  for (auto p = v.begin(); p != v.end(); ++p) {
    out << "(" << p->first << ") "; //<< 
      //" out degree: " << p->second.out_degree() << 
      //" in degree: " << p->second.in_degree() << "\n  ";
    const EdgeList& edges_out = p->second.edges_out();
    for (auto q = edges_out.cbegin(); q != edges_out.cend(); ++q) {
      out << "->(" << q->dst() << ") ";
    }
    const EdgeList edges_in = p->second.edges_in();
    std::cout << "\n  ";
    for (auto q = edges_in.begin(); q != edges_in.end(); ++q) {
      out << "<-(" << q->dst() << ") ";
    }
    out << "\n";
  }
}



int main(int argc, char* argv[]) {
  int size = 5000;
  if (argc > 1) {
    size = std::atoi(argv[1]);
    std::cout << "Size = " << size << " ";
  }

  //Graph graph = read_from_file("example_networks/simple");

  std::chrono::high_resolution_clock::time_point t1a = std::chrono::high_resolution_clock::now();
  std::cout << "Generating graph" << std::endl;
  Graph graph(true);
  random_graph(graph, size, std::min(0.2, 20.0/size));
  std::chrono::high_resolution_clock::time_point t2a = std::chrono::high_resolution_clock::now();
  double difa = std::chrono::duration_cast<std::chrono::seconds>( t2a - t1a ).count();
  std::cout << "Took " << difa << " seconds." << std::endl;

  //filter_edges_on_type(graph, 2);
  //reweigh_edges_by_vertex_and_type(graph);
  reweigh_edges_by_vertex(graph);

  //print_graph(graph, std::cout);

  //dump_to_file(graph, "test");


  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  for (int j = 0; j < 1; ++j) {
    for (int i = 0; i < 1000; ++i) {
      random_walk2(graph, i, 0.85);
    }
  }
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
  std::cout << "Took " << dif << " seconds." << std::endl;
  

  //random_walk(graph, 1, 0.85);
  //random_walk2(graph, 1, 0.85);

  //Graph graph = generate_graph(size);
  //ncomponents(graph);
  //calc_diameter(graph);

  return 0;
}

