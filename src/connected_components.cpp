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
      const EdgeList& edges = p->second.edges();
      for (auto q = edges.cbegin(); q != edges.cend(); ++q) {
        if (components[q->dst()] > components[p->first]) {
          components[q->dst()] = components[p->first];
          iterate = true;
        } else if (components[q->dst()] < components[p->first]) {
          components[p->first] = components[q->dst()];
          iterate = true;
        }
      }
      // TODO test code
    }
  }


  return components;
}

