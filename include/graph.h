#ifndef graph_h
#define graph_h

#include "vertex.h"
#include <vector>

typedef std::vector<Vertex> VertexList;
typedef VertexList::size_type GraphSize;

class Graph {
  public:
    Graph(bool directed = true) : directed_(directed) {
    }

    ~Graph() {
    }

    bool directed() const { return directed_;}

    void directed(bool directed) {
      if (vertices_.size() > 0) 
        throw std::runtime_error("Directedness of a graph can only be changed for a non empty graph.");
      directed_ = directed;
    }

    VertexID add_vertex(const Vertex& vertex) {
      vertices_.push_back(vertex);
      return vertices_.size() - 1;
    }

    void add_vertex(VertexID id, const Vertex& vertex) {
      if (id >= vertices_.size()) {
        vertices_.resize(id+1);
      }
      vertices_[id] = vertex;
    }

    void add_vertex(VertexID id) {
      add_vertex(id, Vertex());
    }

    void add_vertex_if_not_exists(VertexID id) {
      if (id >= vertices_.size()) {
        add_vertex(id, Vertex());
      }
    }

    bool vertex_exists(VertexID id) const {
      return id < vertices_.size();
    }

    const Vertex& vertex(VertexID id) const {
      // TODO: do we need range checking; or also non-range-checked variant?
      return vertices_.at(id);
    }
    
    void add_edge(VertexID from, VertexID to, EdgeWeight weight = 1.0, EdgeType type = 0) {
      Vertex& from_vertex = get_vertex(from);
      from_vertex.add_edge(to, weight, type);
      if (!directed()) {
        Vertex& to_vertex = get_vertex(to);
        to_vertex.add_edge(from, weight, type);
      }
    }

    void remove_edge(VertexID from, VertexID to) {
      Vertex& from_vertex = get_vertex(from);
      from_vertex.remove_edge(to);
      if (!directed()) {
        Vertex& to_vertex = get_vertex(to);
        to_vertex.remove_edge(from);
      }
    }

    const VertexList& vertices() const { return vertices_;}
    VertexList& vertices_nonconst() { return vertices_;}


  private:
    Vertex& get_vertex(VertexID id) {
      return vertices_.at(id);
    }

  private:
    bool directed_;
    VertexList vertices_;
};

#endif
