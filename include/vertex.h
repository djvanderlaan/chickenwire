#ifndef vertex_h
#define vertex_h

#include "types.h"
#include "edge.h"

#include <vector>
#include <stdexcept>


class Vertex {
  public:
    Vertex(vertex_id id = 0, vertex_type type = 0) : id_(id), type_(type) { }
    ~Vertex() { }

    vertex_id id() const { return id_;}

    void type(vertex_type type) { type_ = type;}
    vertex_type type() const { return type_;}

    void add_edge(vertex_id dst, Edge::weight_type weight = 1.0, Edge::type_type type = 0) {
      edges_.push_back(Edge(dst, weight, type));
    }

    void remove_edge(vertex_id dst) {
      EdgeList::iterator p = find_edge(edges_, dst);
      if (p == edges_.end()) 
        throw std::runtime_error("No edge to vertex id " + 
          std::to_string(dst) + " from " + std::to_string(id_) + ".");
      edges_.erase(p);
    }

    bool connected_to(vertex_id vertex) const {
      EdgeList::const_iterator p = find_edge(edges_, vertex);
      return p != edges_.end();
    }

    Edge::weight_type edge_weight(vertex_id vertex) const {
      EdgeList::const_iterator p = find_edge(edges_, vertex);
      return p != edges_.end() ? p->weight() : 0.0;
    }

    EdgeCount degree() const { return edges_.size();}

    const EdgeList& edges() const { return edges_;}
    EdgeList& edges_nonconst() { return edges_;}

  private:
    vertex_id id_;
    vertex_type type_;
    EdgeList edges_;
};

#endif

