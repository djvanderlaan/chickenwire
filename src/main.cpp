#include "graph.h"
#include "functions.h"

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
  write_to_csv(graph, filename);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
  std::cout << "Took " << dif << " seconds." << std::endl;
}

Graph read_from_file(const std::string& filename) {
  std::cout << "Reading from file '" << filename << "'" << std::endl;
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  //Graph graph = read_from_csv(filename);
  Graph graph = graph_from_csv("example_networks/simple_vertices.csv", "example_networks/simple_edges.csv");
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

  Graph graph = read_from_file("foo");
  print_graph(graph, std::cout);

  //Graph graph = generate_graph(size);
  //ncomponents(graph);
  //calc_diameter(graph);

  return 0;
}

