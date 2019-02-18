#ifndef edge_h
#define edge_h

#include "types.h"
#include <vector>

class Edge {
  public:
    typedef edge_type_type type_type;
    typedef edge_weight_type weight_type;

    Edge(vertex_id dst, weight_type weight = 0.0, type_type type = 0) : dst_(dst), 
      weight_(weight), type_(type) {}

    vertex_id dst() const { return dst_;}
    void dst(vertex_id dst) { dst_ = dst;}

    weight_type weight() const { return weight_;}
    void weight(weight_type weight) { weight_ = weight;}

    void type(type_type type) { type_ = type;}
    type_type type() const { return type_;}

  private:
    vertex_id dst_;
    weight_type weight_;
    type_type type_;
};


typedef std::vector<Edge> EdgeList;
typedef EdgeList::size_type EdgeCount;

inline EdgeList::iterator find_edge(EdgeList edges, vertex_id id) {
  EdgeList::iterator p = edges.begin();
  for (; p != edges.end(); ++p) {
    if (p->dst() == id) return p;
  }
  return p;
}

#endif

