#ifndef edge2_h
#define edge2_h

#include "types.h"
#include <vector>

class Edge2 {
  public:

    Edge2(VertexID dst, EdgeWeight weight = 0.0, EdgeType type = 0) : dst_(dst), 
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


typedef std::vector<Edge2> EdgeList2;
typedef EdgeList2::size_type EdgeCount2;

inline EdgeList2::iterator find_edge(EdgeList2 edges, VertexID id) {
  EdgeList2::iterator p = edges.begin();
  for (; p != edges.end(); ++p) {
    if (p->dst() == id) return p;
  }
  return p;
}

#endif

