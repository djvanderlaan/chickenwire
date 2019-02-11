#ifndef graph_h
#define graph_h

#include "vertex.h"
#include <unordered_map>
#include <limits>

//typedef std::unordered_map<vertex_id, Vertex> VertexList;
typedef std::vector<Vertex> VertexList;
typedef VertexList::size_type GraphSize;
typedef std::unordered_map<vertex_id, std::size_t> VertexIDs;
const size_t index_na = std::numeric_limits<std::size_t>::max(); // TODO

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

    void add_vertex(vertex_id id, bool checkid = true) {
      if (checkid) {
        VertexIDs::const_iterator p = vertex_ids_.find(id);
        if (p != vertex_ids_.end()) 
          throw std::runtime_error("Trying to add a vertex with an ID that already exists.");
      } 
      vertex_ids_[id] = vertices_.size();
      vertices_.push_back(Vertex(id));
      //vertices_[id] = Vertex(id);
    }

    void add_vertex_if_not_exists(vertex_id id) {
      VertexIDs::const_iterator p = vertex_ids_.find(id);
      if (p == vertex_ids_.end()) {
        vertex_ids_[id] = vertices_.size();
        vertices_.push_back(Vertex(id));
      }
      //VertexList::const_iterator p = vertices_.find(id);
      //if (p == vertices_.end()) vertices_[id] = Vertex(id);
    }

    bool vertex_exists(vertex_id id) const {
      VertexIDs::const_iterator p = vertex_ids_.find(id);
      return p != vertex_ids_.end();
      //VertexList::const_iterator p = vertices_.find(id);
      //return p != vertices_.end();
    }

    std::size_t get_index_from_id(vertex_id id) {
      VertexIDs::const_iterator p = vertex_ids_.find(id);
      return p != vertex_ids_.end() ? p->second : index_na;
    }

    const Vertex& vertex(std::size_t id) const {
      return vertices_[id];
      //VertexList::const_iterator p = vertices_.find(id);
      //if (p == vertices_.end()) 
        //throw std::runtime_error("Invalid vertex id: " + std::to_string(id) + ".");
      //return p->second;
    }
    
    void add_edge(std::size_t from, std::size_t to, double weight = 1.0) {
      Vertex& from_vertex = get_vertex(from);
      Vertex& to_vertex = get_vertex(to);
      from_vertex.add_out(to, weight);
      to_vertex.add_in(from, weight);
      if (!directed()) {
        from_vertex.add_in(to, weight);
        to_vertex.add_out(from, weight);
      }
    }

    void add_edge_by_id(vertex_id from, vertex_id to, double weight = 1.0) {
      add_edge(get_index_from_id(from), get_index_from_id(to), weight);
    }

    void remove_edge(size_t from, size_t to) {
      Vertex& from_vertex = get_vertex(from);
      Vertex& to_vertex = get_vertex(to);
      from_vertex.remove_out(to);
      to_vertex.remove_in(from);
      if (!directed()) {
        from_vertex.remove_in(to);
        to_vertex.remove_out(from);
      }
    }

    void remove_edge_by_id(vertex_id from, vertex_id to) {
      remove_edge(get_index_from_id(from), get_index_from_id(to));
    }

    const VertexList& vertices() const { return vertices_;}

  private:
    Vertex& get_vertex(std::size_t id) {
      return vertices_[id];
    }
    //Vertex& get_vertex(vertex_id id) {
      //VertexList::iterator p = vertices_.find(id);
      //if (p == vertices_.end()) throw std::runtime_error("Invalid vertex id.");
      //return p->second;
    //}

  private:
    bool directed_;
    VertexList vertices_;
    VertexIDs vertex_ids_;
};

#endif
