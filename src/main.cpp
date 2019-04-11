#include "graph.h"
#include "functions.h"
#include "filter_edges.h"
#include "random_walk.h"
#include "random_graph.h"

#include <functional>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <map>
#include <cstdlib>
#include <unordered_set>
#include <cmath>

inline double runif() {
  return (double)std::rand() / (double)RAND_MAX;
}

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
  std::unordered_set<VertexID> unique_components;
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
    const EdgeList& edges = p->second.edges();
    for (auto q = edges.cbegin(); q != edges.cend(); ++q) {
      out << "->(" << q->dst() << ") ";
    }
    out << "\n";
  }
}


template<typename T>
void measure_time(T fun, const std::string& message = "Starting time measurement") {
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  std::cout << message << std::endl;
  fun();
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  std::cout << "Took " << dif/1000.0 << " seconds." << std::endl;
}



int main(int argc, char* argv[]) {
  int size = 5000;
  if (argc > 1) {
    size = std::atoi(argv[1]);
    std::cout << "Size = " << size << " ";
  }
  std::cout << "\n";

  //Graph graph = read_from_file("example_networks/simple");

  Graph graph(true);
  measure_time([&]() { random_graph(graph, size, std::min(0.2, 100.0/size));}, 
    "Generating graph");

  //filter_edges_on_type(graph, 2);
  //reweigh_edges_by_vertex_and_type(graph);
  reweigh_edges_by_vertex(graph);

  //print_graph(graph, std::cout);

  //dump_to_file(graph, "test");

  /*
  DegreeDistribution ddist = in_degree_distribution(graph);
  for (auto p = ddist.begin(); p != ddist.end(); ++p) {
    std::cout << p->first << ": " << p->second << "\n";
  }

  DegreeMatrix mdist = degree_distribution_matrix(graph);
  for (auto p = mdist.begin(); p != mdist.end(); ++p) {
    std::cout << p->first.first << ", " << p->first.second << ": " << p->second << "\n";
  }

  Components components = connected_components(graph);
  for (auto p = components.begin(); p != components.end(); ++p) {
    std::cout << p->first << ": " << p->second << "\n";
  }
  */

  
  VertexDoubleValues values;
  VertexCategoricalValues cvalues;
  for (auto p = graph.vertices().begin(); p != graph.vertices().end(); ++p) {
    double value = std::trunc(runif()*2);
    //double value = p->second.type();
    //std::cout << p->first << ": value=" << value << "\n";
    values[p->first] = value;
    cvalues[p->first] = value;
    //cvalues[p->first] = p->second.type();
  }
  //measure_time([&](){random_walk_cont2(graph, values);}, "Random Walk CONT");
  //measure_time([&](){random_walk_cat(graph, cvalues);}, "Random Walk CAT");
  //measure_time([&](){random_walk_rev(graph, 7, 0.85);}, "Random Walk");
  //measure_time([&](){random_walk_cont_thread(graph, values, 0.85);}, "Random Walk Thread");
  measure_time([&](){random_walk_threaded_continuous(graph, values, 0.85);}, "Random Walk Threaded");
  measure_time([&](){random_walk_threaded_categorical(graph, cvalues, 0.85);}, "Random Walk Threaded Categorical");

  //random_walk(graph, 1, 0.85);
  //random_walk2(graph, 1, 0.85);

  //Graph graph = generate_graph(size);
  //ncomponents(graph);
  //calc_diameter(graph);

  return 0;
}

