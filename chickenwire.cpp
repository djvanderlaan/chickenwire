
#include "graph.h"
#include "random_walk.h"
#include "shortest_path.h"
#include "connected_components.h"
#include "degree_distribution.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <chrono>

inline unsigned int sample(unsigned int max) {
  return std::rand() % max;
}


// ============================================================================
// ============================================================================
// ============================================================================


int main(int argc, char* argv[]) {
  unsigned int size = 5000;
  if (argc > 1) {
    size = std::atoi(argv[1]);
  }
  std::cout << "Size = " << size << "\n";


  const unsigned int nedge = 10;

  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  Graph graph(true);
  for (unsigned int i = 0; i < size; ++i) {
    Vertex v;
    for (unsigned int j = 0; j < nedge; ++j) {
      v.add_edge(sample(size));
    }
    graph.add_vertex(v);
  }
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  std::cout << "Generation took " << dif/1000.0 << " seconds." << std::endl;

  if (false) {
    VertexDoubleValues values(size);
    VertexWeights weights(size);
    double sum = 0.0;
    for (unsigned int i = 0; i < size; ++i) { 
      const double r = ((double)std::rand() / RAND_MAX);
      values[i] = r >  0.1 ? 0 : 1;
      weights[i] = 2.0;
      sum += values[i];
    }
    std::cout << "MEAN=" << sum/size << "\n";
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    VertexDoubleValues rw = random_walk_continuous(graph, values, 0.85, 2);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "RW computation took " << dif/1000.0 << " seconds." << std::endl;
    for (size_t i = 0; i < size; ++i) {
      std::cout << i << "\t\t" << rw[i] << "\n";
      if (i > 10) break;
    }
  }
  
  if (false) {
    VertexCategoricalValues values(size);
    VertexWeights weights(size);
    double sum = 0.0;
    for (unsigned int i = 0; i < size; ++i) { 
      const double r = ((double)std::rand() / RAND_MAX);
      values[i] = r > 0.1 ? 0 : 1;
      weights[i] = 2.0;
      if (r > 0.85) values[i] = 2;
      sum += values[i];
    }
    std::cout << "MEAN=" << sum/size << "\n";
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    RandomWalkResult rw = random_walk_categorical(graph, values, 0.85, 2);
    std::cout << rw.size() << "\n";
    for (size_t i = 0; i < rw[0].size(); ++i) {
      std::cout << i;
      for (size_t col = 0; col < rw.size(); ++col) {
        std::cout << "\t" << rw[col][i] ;
      }
      std::cout << "\n";
      if (i > 10) break;
    }
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "RW computation took " << dif/1000.0 << " seconds." << std::endl;
  }

  {
    std::cout << "Shortest path length\n";
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    double res = shortest_path_length(graph, 1, 2);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
    std::cout << "Path length: " << res << "\n";
  }

  {
    std::cout << "Shortest path\n";
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    Path res = shortest_path(graph, 1, 2);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
    for (size_t i = 0; i < res.size(); ++i) {
      auto e = res[i];
      std::cout << e.vertex_id << "(" << e.path_length << ") ";
      if (i > 10) break;
    }
    std::cout << "\n";
  }

  {
    std::cout << "All shortest path lengths\n";
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    PathLengths res = all_shortest_path_lengths(graph, 1);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
    for (size_t i = 0; i < res.size(); ++i) {
      std::cout << i << "\t\t" << res[i] << "\n";
      if (i > 10) break;
    }
  }

  {
    std::cout << "All shortest paths\n";
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    Graph res = all_shortest_paths(graph, 1);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
    // Output path from 2 to 1 
    VertexID id = 2;
    for (size_t i = 0; i < 20; ++i) {
      const Vertex& vertex = res.vertex(id);
      const EdgeList& edges = vertex.edges();
      for (auto p = edges.cbegin(); p != edges.cend(); ++p) {
        std::cout << id << "(" << p->weight() << ") ";
        id = p->dst();
      }
      if (id == 1) break;
    }
    std::cout << "\n";
  }

  if (false) {
    std::cout << "Diameter\n";
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    double res = diameter(graph);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
    std::cout << "Max shortest path: " << res << "\n";
  }

  {
    std::cout << "Connected components\n";
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    size_t res = nconnected_components(graph);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
    std::cout << "Number of components: " << res << "\n";
  }

  {
    std::cout << "Mean degree\n";
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    double resin = mean_in_degree(graph);
    double resout = mean_out_degree(graph);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
    std::cout << "Indegree: " << resin << " out=" << resout << "\n";
  }

  return 0;
}

