#ifndef graph2_h
#define graph2_h

#include "vertex2.h"
#include <unordered_map>

typedef std::unordered_map<VertexID, Vertex2> VertexList2;

class Graph2 {
  public:
    Graph2(bool directed = true) : directed_(directed) {
    }

    ~Graph2() {
    }

    bool directed() const { return directed_;}

    void directed(bool directed) {
      if (vertices_.size() > 0) 
        throw std::runtime_error("Directedness of a graph can only be changed for a non empty graph.");
      directed_ = directed;
    }

    void add_vertex(VertexID id) {
      vertices_[id] = Vertex2(id);
    }

    void add_vertex(const Vertex2& vertex) {
      vertices_[vertex.id()] = vertex;
    }

    void add_vertex_if_not_exists(VertexID id) {
      VertexList2::const_iterator p = vertices_.find(id);
      if (p == vertices_.end()) vertices_[id] = Vertex2(id);
    }

    bool vertex_exists(VertexID id) const {
      VertexList2::const_iterator p = vertices_.find(id);
      return p != vertices_.end();
    }

    const Vertex2& vertex(VertexID id) const {
      VertexList2::const_iterator p = vertices_.find(id);
      if (p == vertices_.end()) 
        throw std::runtime_error("Invalid vertex id: " + std::to_string(id) + ".");
      return p->second;
    }
    
    void add_edge(VertexID from, VertexID to, EdgeWeight weight = 1.0, EdgeType type = 0) {
      Vertex2& from_vertex = get_vertex(from);
      Vertex2& to_vertex = get_vertex(to);
      from_vertex.add_edge(to, weight, type);
      if (!directed()) {
        to_vertex.add_edge(from, weight, type);
      }
    }

    void remove_edge(VertexID from, VertexID to) {
      Vertex2& from_vertex = get_vertex(from);
      Vertex2& to_vertex = get_vertex(to);
      from_vertex.remove_edge(to);
      if (!directed()) {
        to_vertex.remove_edge(from);
      }
    }

    const VertexList2& vertices() const { return vertices_;}
    VertexList2& vertices_nonconst() { return vertices_;}


  private:
    Vertex2& get_vertex(VertexID id) {
      VertexList2::iterator p = vertices_.find(id);
      if (p == vertices_.end()) throw std::runtime_error("Invalid vertex id.");
      return p->second;
    }

  private:
    bool directed_;
    VertexList2 vertices_;
};

#endif
