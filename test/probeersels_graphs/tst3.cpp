#include <iostream>
#include <iomanip>
#include <vector>
#include <exception>
#include <random>
#include <chrono>

class Graph {
  public:
    Graph(size_t nvertices) : nvertices_(nvertices), vertices_(nvertices, -1L), 
      out_degree_(nvertices, 0L) {
    }

    ~Graph() {
    }

    void add_edge(int src, int dst, float weight = 1.0) {
      if (src < current_vertex_for_adding_edges_) {
        throw std::runtime_error("Edges should be added in vertex order.");
      } else if (src > current_vertex_for_adding_edges_) {
        current_vertex_for_adding_edges_ = src;
        vertices_[src] = out_edges_.size();
      } 
      ++out_degree_[src];
      out_edges_.push_back(dst);
      out_weights_.push_back(weight);
    }



    class Edge {
      public:
        Edge(Graph* graph, size_t pos = 0, int src = 0, int src_pos = 0) : graph_(graph), 
          pos_(pos), src_(src), src_pos_(src_pos) {}

        int src() { return src_; }

        int dst() { return graph_->out_edges_[pos_]; }

        void next() {
          ++pos_;
          ++src_pos_;
          while (src_pos_ >= graph_->out_degree_[src_]) {
            ++src_; 
            src_pos_ = 0;
          }
        }

        bool operator==(const Edge& b) const {
          return pos_ == b.pos_;
        }
        bool operator!=(const Edge& b) const {
          return pos_ != b.pos_;
        }
        void operator++() {
          next();
        }

      private:
        Graph* graph_ = 0;
        size_t pos_ = 0;
        int src_ = 0;
        int src_pos_ = 0;
    };

    Edge begin() {
      return Edge{this, 0L, 0L};
    }
    Edge end() {
      return Edge{this, out_edges_.size()};
    }


  public:
    size_t nvertices_;
    std::vector<int> out_edges_;       // size nedges
    std::vector<float> out_weights_;   // size nedges
    std::vector<int> vertices_;        // size nvertices
    std::vector<int> out_degree_;      // size nvertices
    int current_vertex_for_adding_edges_ = -1;
};

std::vector<int> connected_components(Graph& graph) {
  std::vector<int> components;
  components.reserve(graph.nvertices_);
  for (int i = 0; i < graph.nvertices_; ++i) components[i] = i;
  bool changed = true;
  int iteration = 0;
  while (changed) {
    changed = false;
    for (Graph::Edge e = graph.begin(), end = graph.end(); e != end; ++e) {
      int dst_component = components[e.dst()];
      int src_component = components[e.src()];
      if (dst_component > src_component) {
        components[e.dst()] = src_component;
        changed = true;
      } else if (dst_component < src_component) {
        components[e.src()] = dst_component;
        changed = true;
      }
    }
    ++iteration;
  }
  std::cout << "iterations = " << iteration << "\n";
  return components;
}


int main(int argc, char* argv[]) {
  int n = 2E6;
  int k = 200;
  if (argc > 1) n = std::atoi(argv[1]);
  if (argc > 2) k = std::atoi(argv[2]);
  std::cout << "n = " << n << std::endl;
  std::cout << "k = " << k << std::endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> rint(0, n-1);
  std::uniform_int_distribution<> rdegree(0, k);

  Graph graph(n);

  for (int i = 0; i < n; ++i) {
    int k = rdegree(gen);
    for (int j = 0; j < k; ++j) {
      int dst = rint(gen);
      graph.add_edge(i, dst, 1.0);
    }
  }

  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  std::vector<int> components = connected_components(graph);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
  std::cout << "connected components took " << dif/1000.0 << " seconds." << std::endl;

  for (int i = 0; i < 10; ++i) {
    std::cout << i << ": " << components[i] << "\n";
  }

  //for (auto e = graph.begin(); e != graph.end(); ++e) {
    //std::cout << e.src() << "->" << e.dst() << "\n";
  //}
 
  return 0;
}

