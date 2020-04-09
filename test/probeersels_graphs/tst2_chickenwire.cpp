#include "chickenwire/graph.h"
//#include "chickenwire/connected_components.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <random>
#include <algorithm>

std::vector<VertexID> connected_components(const Graph& graph) {
  const VertexList& vertices = graph.vertices();

  // Initialise components: put each vertex in its own component, labelled 
  // conveniently equal to the vertexid
  std::vector<VertexID> components(vertices.size());
  for (VertexID i = 0; i < vertices.size(); ++i) components[i] = i;

  bool iterate = true;
  int iteration = 0;
  while (iterate) {
    iterate = false;
    VertexID i = 0;
    for (auto p = vertices.cbegin(); p != vertices.cend(); ++p, ++i) {
      const EdgeList& edges = p->edges();
      VertexID id_src = components[i];
      for (auto q = edges.cbegin(); q != edges.cend(); ++q) {
        VertexID id_dst = components[q->dst()];
        if (id_dst > id_src) {
          components[q->dst()] = id_src;
          iterate = true;
        } else if (id_dst < id_src) {
          components[i] = id_dst;
          iterate = true;
        }
      }
    }
    ++iteration;
  }
  std::cout << "iterations = " << iteration << "\n";
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

  Graph graph;

  std::vector<int> vertices;
  for (int i = 0; i < n; ++i) {
    graph.add_vertex(i);
    vertices.push_back(i);
  }

  //std::shuffle(vertices.begin(), vertices.end(), gen);

  /*for (int i = 0; i < n; ++i) {
    int k = rdegree(gen);
    for (int j = 0; j < k; ++j) {
      int dst = rint(gen);
      graph.add_edge(i, dst, 1.0);
    }
  }*/
  for (int i: vertices) {
  //for (int i = 0; i < n; ++i) {
    int k = rdegree(gen);
    for (int j = 0; j < k; ++j) {
      int dst = rint(gen);
      graph.add_edge(i, dst, 1.0);
    }
  }

  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  std::vector<VertexID> components = connected_components(graph);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  std::cout << "Connected components took " << dif/1000.0 << " seconds." << std::endl;

  //for (int i = 0; i < graph.nvertices_; ++i) {
  for (int i = 0; i < 10; ++i) {
    std::cout << i << ": " << components[i] << "\n";
  }

  return 0;
}

