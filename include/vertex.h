#ifndef vertex_h
#define vertex_h

#include "types.h"
#include "edge.h"

#include <vector>
#include <stdexcept>


class Vertex {
  public:
    Vertex(VertexID id = 0, VertexType type = 0) : id_(id), type_(type) { }
    ~Vertex() { }

    VertexID id() const { return id_;}

    void type(VertexType type) { type_ = type;}
    VertexType type() const { return type_;}

    void add_edge(VertexID dst, EdgeWeight weight = 1.0, EdgeType type = 0) {
      edges_.push_back(Edge(dst, weight, type));
    }

    void remove_edge(VertexID dst) {
      EdgeList::iterator p = find_edge(edges_, dst);
      if (p == edges_.end()) 
        throw std::runtime_error("No edge to vertex id " + 
          std::to_string(dst) + " from " + std::to_string(id_) + ".");
      edges_.erase(p);
    }

    bool connected_to(VertexID vertex) const {
      EdgeList::const_iterator p = find_edge(edges_, vertex);
      return p != edges_.end();
    }

    EdgeWeight edge_weight(VertexID vertex) const {
      EdgeList::const_iterator p = find_edge(edges_, vertex);
      return p != edges_.end() ? p->weight() : 0.0;
    }

    EdgeCount degree() const { return edges_.size();}

    const EdgeList& edges() const { return edges_;}
    EdgeList& edges_nonconst() { return edges_;}

  private:
    VertexID id_;
    VertexType type_;
    EdgeList edges_;
};

#endif

