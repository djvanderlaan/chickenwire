#ifndef edge_h
#define edge_h

#include "types.h"

#include <vector>

class Edge {
  public:
    Edge(vertex_id dst, edge_weight weight = 0.0, edge_type type = 0) : dst_(dst), 
      weight_(weight), type_(type) {}

    vertex_id dst() const { return dst_;}
    void dst(vertex_id dst) { dst_ = dst;}

    edge_weight weight() const { return weight_;}
    void weight(edge_weight weight) { weight_ = weight;}

    void type(edge_type type) { type_ = type;}
    edge_type type() const { return type_;}


  private:
    vertex_id dst_;
    edge_weight weight_;
    edge_type type_;
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

