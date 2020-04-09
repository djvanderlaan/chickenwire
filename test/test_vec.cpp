#include <iostream>
#include <vector>


class Graph {
  public:

    void add_vertex(int src) {
      if (degrees.size() < (src+1)) {
        degrees.resize(src+1, 0L);
        indices.resize(src+1, 0L);
        if (src > 0) {
          indices[src] = indices[src-1] + degrees[src-1];
        }
      }
    }

    void add_edge(int src, int dst) {
      if (degrees.size() < (src+1)) {
        degrees.resize(src+1, 0L);
        indices.resize(src+1, 0L);
      }
      degrees[src] += 1L;
      recalc_indices();
      edges.insert(edges.begin() + (indices[src] + degrees[src] - 1L), dst);
    }

    void recalc_indices() {
      std::vector<int>::iterator p = indices.begin();
      int sum = 0;
      for (std::vector<int>::const_iterator q = degrees.begin(), qend = degrees.end(); q != qend; ++q, ++p) {
        *p = sum;
        sum += *q;
      }
    }

  public:
    std::vector<int> degrees;
    std::vector<int> indices;
    std::vector<int> edges;
};

int main(int argc, char* argv[]) {
  Graph graph;


  graph.add_vertex(15);
  graph.add_edge(10, 10);
  graph.add_edge(10, 11);
  graph.add_edge(20, 20);
  graph.add_vertex(25);



  auto i = graph.indices.begin();
  auto k = graph.degrees.begin();

  for (int src = 0; src < graph.degrees.size(); ++src, ++i, ++k) {
    std::cout << src << "\t k = " << *k << "\n";
    for (int j = 0; j < *k; ++j) {
      std::cout << "     " << src << "->" << graph.edges[*i+j] << "\n";
    }
  }

  return 0;
}

