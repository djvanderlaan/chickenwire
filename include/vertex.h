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

    // ==== Edge manipulation
    void add_out(vertex_id out, edge_weight weight = 1.0, edge_type type = 0) {
      out_.push_back(Edge(out, weight, type));
    }

    void add_in(vertex_id in, edge_weight weight = 1.0, edge_type type = 0) {
      in_.push_back(Edge(in, weight, type));
    }

    void remove_out(vertex_id out) {
      EdgeList::iterator p = find_edge(out_, out);
      if (p == out_.end()) 
        throw std::runtime_error("No out edge to vertex id " + 
          std::to_string(out) + " from " + std::to_string(id_) + ".");
      out_.erase(p);
    }

    void remove_in(vertex_id in) {
      EdgeList::iterator p = find_edge(in_, in);
      if (p == in_.end()) 
        throw std::runtime_error("No in edge to vertex id " + 
          std::to_string(in) + " from " + std::to_string(id_) + ".");
      in_.erase(p);
    }

    bool connected_to(vertex_id vertex) const {
      EdgeList::const_iterator p = find_edge(out_, vertex);
      return p != out_.end();
    }

    edge_weight out_edge_weight(vertex_id vertex) const {
      EdgeList::const_iterator p = find_edge(out_, vertex);
      return p != out_.end() ? p->weight() : 0.0;
    }

    edge_weight in_edge_weight(vertex_id vertex) const {
      EdgeList::const_iterator p = find_edge(out_, vertex);
      return p != in_.end() ? p->weight() : 0.0;
    }

    EdgeCount in_degree() const { return in_.size();}
    EdgeCount out_degree() const { return out_.size();}

    const EdgeList& edges_out() const { return out_;}
    const EdgeList& edges_in() const { return in_;}

    EdgeList& edges_out_nonconst() { return out_;}
    EdgeList& edges_in_nonconst() { return in_;}

  private:
    vertex_id id_;
    vertex_type type_;
    EdgeList in_;
    EdgeList out_;
};

#endif
