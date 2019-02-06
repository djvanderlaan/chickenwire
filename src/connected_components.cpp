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
    components[p->first] = p->first;

  bool iterate = true;
  while(iterate) {
    iterate = false;
    for (auto p = vertices.begin(); p != vertices.end(); ++p) {
      const EdgeList& edges_out = p->second.edges_out();
      for (auto q = edges_out.cbegin(); q != edges_out.cend(); ++q) {
        if (components[q->first] > components[p->first]) {
          components[q->first] = components[p->first];
          iterate = true;
        }
      }
      const EdgeList& edges_in = p->second.edges_in();
      for (auto q = edges_in.cbegin(); q != edges_in.cend(); ++q) {
        if (components[q->first] > components[p->first]) {
          components[q->first] = components[p->first];
          iterate = true;
        }
      }
    }
  }


  return components;
}

