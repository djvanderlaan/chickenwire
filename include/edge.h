#ifndef edge_h
#define edge_h

#include "types.h"
#include <vector>

class Edge {
  public:

    Edge(VertexID dst, EdgeWeight weight = 0.0, EdgeType type = 0) : dst_(dst), 
      weight_(weight), type_(type) {}

    VertexID dst() const { return dst_;}
    void dst(VertexID dst) { dst_ = dst;}

    EdgeWeight weight() const { return weight_;}
    void weight(EdgeWeight weight) { weight_ = weight;}

    void type(EdgeType type) { type_ = type;}
    EdgeType type() const { return type_;}

  private:
    VertexID dst_;
    EdgeWeight weight_;
    EdgeType type_;
};


typedef std::vector<Edge> EdgeList;
typedef EdgeList::size_type EdgeCount;

inline EdgeList::iterator find_edge(EdgeList edges, VertexID id) {
  EdgeList::iterator p = edges.begin();
  for (; p != edges.end(); ++p) {
    if (p->dst() == id) return p;
  }
  return p;
}

#endif

