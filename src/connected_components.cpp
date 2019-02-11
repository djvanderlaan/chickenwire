#include "functions.h"

#include<unordered_map>
#include<thread>
#include<mutex>
#include<functional>
#include<cmath>
#include<vector>

#include<iostream>
#include<iomanip>

Components connected_components(const Graph& graph) {
  Components components;
  
  // Initialise components
  const VertexList& vertices = graph.vertices();
  for (auto p = vertices.begin(); p != vertices.end(); ++p) 
    components[p->id()] = p->id();

  bool iterate = true;
  while(iterate) {
    iterate = false;
    for (auto p = vertices.begin(); p != vertices.end(); ++p) {
      const EdgeList& edges_out = p->edges_out();
      for (auto q = edges_out.cbegin(); q != edges_out.cend(); ++q) {
        if (components[q->dst()] > components[p->id()]) {
          components[q->dst()] = components[p->id()];
          iterate = true;
        }
      }
      const EdgeList& edges_in = p->edges_in();
      for (auto q = edges_in.cbegin(); q != edges_in.cend(); ++q) {
        if (components[q->dst()] > components[p->id()]) {
          components[q->dst()] = components[p->id()];
          iterate = true;
        }
      }
    }
  }


  return components;
}

