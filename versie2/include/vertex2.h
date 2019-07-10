#ifndef vertex2_h
#define vertex2_h

#include "types.h"
#include "edge2.h"

#include <vector>
#include <stdexcept>


class Vertex2 {
  public:
    Vertex2(VertexID id = 0, VertexType type = 0) : id_(id), type_(type) { }
    ~Vertex2() { }

    VertexID id() const { return id_;}

    void type(VertexType type) { type_ = type;}
    VertexType type() const { return type_;}

    void add_edge(VertexID dst, EdgeWeight weight = 1.0, EdgeType type = 0) {
      edges_.push_back(Edge2(dst, weight, type));
    }

    void remove_edge(VertexID dst) {
      EdgeList2::iterator p = find_edge(edges_, dst);
      if (p == edges_.end()) 
        throw std::runtime_error("No edge to vertex id " + 
          std::to_string(dst) + " from " + std::to_string(id_) + ".");
      edges_.erase(p);
    }

    bool connected_to(VertexID vertex) const {
      EdgeList2::const_iterator p = find_edge(edges_, vertex);
      return p != edges_.end();
    }

    EdgeWeight edge_weight(VertexID vertex) const {
      EdgeList2::const_iterator p = find_edge(edges_, vertex);
      return p != edges_.end() ? p->weight() : 0.0;
    }

    EdgeCount2 degree() const { return edges_.size();}

    const EdgeList2& edges() const { return edges_;}
    EdgeList2& edges_nonconst() { return edges_;}

  private:
    VertexID id_;
    VertexType type_;
    EdgeList2 edges_;
};

#endif

