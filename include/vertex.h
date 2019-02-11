#ifndef vertex_h
#define vertex_h

//#include <unordered_map>
#include <vector>
#include <stdexcept>

typedef int vertex_id;

class Edge {
  public:
    Edge(vertex_id dst, double weight = 0.0) : dst_(dst), weight_(weight) {}

    vertex_id dst() const { return dst_;}
    void dst(vertex_id dst) { dst_ = dst;}

    double weight() const { return weight_;}
    void weight(double weight) { weight_ = weight;}

  private:
    vertex_id dst_;
    double weight_;
};

typedef std::vector<Edge> EdgeList;
//typedef std::unordered_map<vertex_id, double> EdgeList;
typedef EdgeList::size_type EdgeCount;


inline EdgeList::iterator find_edge(EdgeList edges, vertex_id id) {
  EdgeList::iterator p = edges.begin();
  for (; p != edges.end(); ++p) {
    if (p->dst() == id) return p;
  }
  return p;
}


class Vertex {
  public:
    Vertex(vertex_id id = 0) : id_(id) { }
    ~Vertex() { }

    vertex_id id() const { return id_;}

    void add_out(vertex_id out, double weight = 1.0) {
      out_.push_back(Edge(out, weight));
      //out_.insert(std::make_pair(out, weight));
    }

    void add_in(vertex_id in, double weight = 1.0) {
      //in_.insert(std::make_pair(in, weight));
      in_.push_back(Edge(in, weight));
    }

    void remove_out(vertex_id out) {
      EdgeList::iterator p = find_edge(out_, out);
      //EdgeList::iterator p = out_.find(out);
      if (p == out_.end()) 
        throw std::runtime_error("No out edge to given vertex id.");
      out_.erase(p);
    }

    void remove_in(vertex_id in) {
      EdgeList::iterator p = find_edge(in_, in);
      //EdgeList::iterator p = in_.find(in);
      if (p == in_.end()) 
        throw std::runtime_error("No in edge to given vertex id.");
      in_.erase(p);
    }

    bool connected_to(vertex_id vertex) const {
      //EdgeList::const_iterator p = out_.find(vertex);
      EdgeList::const_iterator p = find_edge(out_, vertex);
      return p != out_.end();
    }

    double out_edge_weight(vertex_id vertex) const {
      //EdgeList::const_iterator p = out_.find(vertex);
      EdgeList::const_iterator p = find_edge(out_, vertex);
      return p != out_.end() ? p->weight() : 0.0;
    }

    double in_edge_weight(vertex_id vertex) const {
      //EdgeList::const_iterator p = in_.find(vertex);
      EdgeList::const_iterator p = find_edge(out_, vertex);
      return p != in_.end() ? p->weight() : 0.0;
    }

    EdgeCount in_degree() const { return in_.size();}
    EdgeCount out_degree() const { return out_.size();}

    const EdgeList& edges_out() const { return out_;}
    const EdgeList& edges_in() const { return in_;}

  private:
    vertex_id id_;
    EdgeList in_;
    EdgeList out_;
};

#endif
