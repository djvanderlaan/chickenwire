#ifndef graph_h
#define graph_h

#include "vertex.h"
#include <unordered_map>

typedef std::unordered_map<vertex_id, Vertex> VertexList;
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

    void add_vertex(vertex_id id) {
      vertices_[id] = Vertex(id);
    }

    void add_vertex(const Vertex& vertex) {
      vertices_[vertex.id()] = vertex;
    }

    void add_vertex_if_not_exists(vertex_id id) {
      VertexList::const_iterator p = vertices_.find(id);
      if (p == vertices_.end()) vertices_[id] = Vertex(id);
    }

    bool vertex_exists(vertex_id id) const {
      VertexList::const_iterator p = vertices_.find(id);
      return p != vertices_.end();
    }

    const Vertex& vertex(vertex_id id) const {
      VertexList::const_iterator p = vertices_.find(id);
      if (p == vertices_.end()) 
        throw std::runtime_error("Invalid vertex id: " + std::to_string(id) + ".");
      return p->second;
    }
    
    void add_edge(vertex_id from, vertex_id to, edge_weight weight = 1.0, edge_type type = 0) {
      Vertex& from_vertex = get_vertex(from);
      Vertex& to_vertex = get_vertex(to);
      from_vertex.add_out(to, weight, type);
      to_vertex.add_in(from, weight, type);
      if (!directed()) {
        from_vertex.add_in(to, weight, type);
        to_vertex.add_out(from, weight, type);
      }
    }

    void remove_edge(vertex_id from, vertex_id to) {
      Vertex& from_vertex = get_vertex(from);
      Vertex& to_vertex = get_vertex(to);
      from_vertex.remove_out(to);
      to_vertex.remove_in(from);
      if (!directed()) {
        from_vertex.remove_in(to);
        to_vertex.remove_out(from);
      }
    }

    const VertexList& vertices() const { return vertices_;}

  private:
    Vertex& get_vertex(vertex_id id) {
      VertexList::iterator p = vertices_.find(id);
      if (p == vertices_.end()) throw std::runtime_error("Invalid vertex id.");
      return p->second;
    }

  private:
    bool directed_;
    VertexList vertices_;
};

#endif
