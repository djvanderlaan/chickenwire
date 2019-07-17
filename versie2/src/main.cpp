
#include "graph.h"
#include "graph2.h"
#include "random_walk.h"
#include "shortest_path.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <chrono>

inline unsigned int sample(unsigned int max) {
  return std::rand() % max;
}

// ============================================================================
// ============================================================================
// ============================================================================

void my_computation(const Graph& graph) {
  const VertexList& vertices = graph.vertices();
  double total_degree = 0.0;
  double n = 0.0;
  for (auto p = vertices.cbegin(), end = vertices.cend(); p != end; ++p) {
    const EdgeList& edges = p->edges();
    for (auto q = edges.cbegin(), qend = edges.cend(); q != qend; ++q) 
      total_degree += q->weight();
    n += 1.0;
  }
  std::cout << total_degree / n << "\n";
}

void my_computation(const Graph2& graph) {
  const VertexList2& vertices = graph.vertices();
  double total_degree = 0.0;
  double n = 0.0;
  for (auto p = vertices.cbegin(), end = vertices.cend(); p != end; ++p) {
    //total_degree += p->second.degree();
    const EdgeList2& edges = p->second.edges();
    for (auto q = edges.cbegin(), qend = edges.cend(); q != qend; ++q) {
      total_degree += q->weight();
    }
    n += 1.0;
  }
  std::cout << total_degree / n << "\n";
}


// ============================================================================
// ============================================================================
// ============================================================================

#include "stepper.h"
#include "chunker.h"

class ParallelComputation {
  public:
    ParallelComputation(unsigned int nworkers, const VertexList& vertices) :
        chunks_(vertices, nworkers), total_(0), n_(0) {
      total_ = new double[nworkers];
      if (total_ == 0) throw std::runtime_error("Allocation failed");
      n_ = new double[nworkers];
      if (n_ == 0) throw std::runtime_error("Allocation failed");
    }

    ~ParallelComputation() {
      delete [] total_;
      delete [] n_;
    }

    bool operator()(int step, unsigned int id, unsigned int nworkers) {
      total_[id] = 0.0;
      n_[id] = 0.0;
      for (auto p = chunks_.begin(id); p != chunks_.end(id); ++p) {

        const EdgeList& edges = p->edges();
        for (auto q = edges.cbegin(), qend = edges.cend(); q != qend; ++q) {
          total_[id] += q->weight();
        }
        n_[id] += 1.0;
      }
      return false;
    }

    double result() {
      double total = 0.0;
      double n = 0.0;
      for (unsigned int i = 0; i < chunks_.nchunks(); ++i) {
        total += total_[i];
        n += n_[i];
      }
      return total/n;
    }
  private:
    Chunker<VertexList> chunks_;
    double* total_;
    double* n_;
};

void my_parallel_computation(const Graph& graph) {
  unsigned int nworkers = determine_nthreads(graph.vertices().size());
  ParallelComputation comp(nworkers, graph.vertices());
  Stepper<ParallelComputation> stepper(comp);
  stepper.run(nworkers, 100);
  std::cout << comp.result() << "\n";
}

class ParallelComputation2 {
  public:
    ParallelComputation2(unsigned int nworkers, const VertexList2& vertices) :
        chunks_(vertices, nworkers), total_(0), n_(0) {
      total_ = new double[nworkers];
      n_ = new double[nworkers];
    }

    ~ParallelComputation2() {
      delete [] total_;
      delete [] n_;
    }

    bool operator()(int step, unsigned int id, unsigned int nworkers) {
      total_[id] = 0.0;
      n_[id] = 0.0;
      for (auto p = chunks_.begin(id); p != chunks_.end(id); ++p) {

        const EdgeList2& edges = p->second.edges();
        for (auto q = edges.cbegin(), qend = edges.cend(); q != qend; ++q) {
          total_[id] += q->weight();
        }
        n_[id] += 1.0;
      }
      return false;
    }

    double result() {
      double total = 0.0;
      double n = 0.0;
      for (unsigned int i = 0; i < chunks_.nchunks(); ++i) {
        total += total_[i];
        n += n_[i];
      }
      return total/n;
    }
  private:
    Chunker<VertexList2> chunks_;
    double* total_;
    double* n_;
};

void my_parallel_computation2(const Graph2& graph) {
  unsigned int nworkers = determine_nthreads(graph.vertices().size());
  ParallelComputation2 comp(nworkers, graph.vertices());
  Stepper<ParallelComputation2> stepper(comp);
  stepper.run(nworkers, 100);
  std::cout << comp.result() << "\n";
}


// ============================================================================
// ============================================================================
// ============================================================================


int main(int argc, char* argv[]) {
  unsigned int size = 5000;
  if (argc > 1) {
    size = std::atoi(argv[1]);
  }
  std::cout << "Size = " << size << "\n";
  int versie = 1;
  if (argc > 2) {
    if (std::string(argv[2]) == "1") {
      versie = 1;
    } else if (std::string(argv[2]) == "2") {
      versie = 2;
    } else {
      throw std::runtime_error("Version can be 1/2.");
    }
  }
  std::cout << "Version = " << versie << "\n";


  const unsigned int nedge = 10;

  if (versie == 1) {
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    Graph graph(true);
    for (unsigned int i = 0; i < size; ++i) {
      Vertex v;
      for (unsigned int j = 0; j < nedge; ++j) {
        v.add_edge(sample(size));
      }
      graph.add_vertex(v);
    }
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Generation took " << dif/1000.0 << " seconds." << std::endl;

    /*{
      std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
      my_computation(graph);
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
      double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;

      t1 = std::chrono::high_resolution_clock::now();
      my_parallel_computation(graph);
      t2 = std::chrono::high_resolution_clock::now();
      dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "Parallel computation took " << dif/1000.0 << " seconds." << std::endl;
    }*/

    if (false) {
      VertexDoubleValues values(size);
      VertexWeights weights(size);
      double sum = 0.0;
      for (unsigned int i = 0; i < size; ++i) { 
        const double r = ((double)std::rand() / RAND_MAX);
        values[i] = r >  0.1 ? 0 : 1;
        weights[i] = 2.0;
        sum += values[i];
      }
      std::cout << "MEAN=" << sum/size << "\n";
      std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
      VertexDoubleValues rw = random_walk_continuous(graph, values, 0.85, 2);
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
      double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "RW computation took " << dif/1000.0 << " seconds." << std::endl;
      for (size_t i = 0; i < size; ++i) {
        std::cout << i << "\t\t" << rw[i] << "\n";
        if (i > 10) break;
      }
    }
    
    if (false) {
      VertexCategoricalValues values(size);
      VertexWeights weights(size);
      double sum = 0.0;
      for (unsigned int i = 0; i < size; ++i) { 
        const double r = ((double)std::rand() / RAND_MAX);
        values[i] = r > 0.1 ? 0 : 1;
        weights[i] = 2.0;
        if (r > 0.85) values[i] = 2;
        sum += values[i];
      }
      std::cout << "MEAN=" << sum/size << "\n";
      std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
      RandomWalkResult rw = random_walk_categorical(graph, values, 0.85, 2);
      std::cout << rw.size() << "\n";
      for (size_t i = 0; i < rw[0].size(); ++i) {
        std::cout << i;
        for (size_t col = 0; col < rw.size(); ++col) {
          std::cout << "\t" << rw[col][i] ;
        }
        std::cout << "\n";
        if (i > 10) break;
      }
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
      double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "RW computation took " << dif/1000.0 << " seconds." << std::endl;
    }

    {
      std::cout << "Shortest path length orig\n";
      std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
      double res = shortest_path_length(graph, 1, 2);
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
      double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
      std::cout << "Path length: " << res << "\n";
    }

    {
      std::cout << "Shortest path length V2\n";
      std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
      double res = shortest_path_length2(graph, 1, 2);
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
      double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
      std::cout << "Path length: " << res << "\n";
    }

    {
      std::cout << "Shortest path orig\n";
      std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
      Path res = shortest_path(graph, 1, 2);
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
      double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
      for (size_t i = 0; i < res.size(); ++i) {
        auto e = res[i];
        std::cout << e.vertex_id << "(" << e.path_length << ") ";
        if (i > 10) break;
      }
      std::cout << "\n";
    }

    {
      std::cout << "Shortest path V2\n";
      std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
      Path res = shortest_path2(graph, 1, 2);
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
      double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
      for (size_t i = 0; i < res.size(); ++i) {
        auto e = res[i];
        std::cout << e.vertex_id << "(" << e.path_length << ") ";
        if (i > 10) break;
      }
      std::cout << "\n";
    }

    {
      std::cout << "All shortest path lengths\n";
      std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
      PathLengths res = all_shortest_path_lengths(graph, 1);
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
      double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
      for (size_t i = 0; i < res.size(); ++i) {
        std::cout << i << "\t\t" << res[i] << "\n";
        if (i > 10) break;
      }
    }

    {
      std::cout << "All shortest path lengths V2\n";
      std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
      PathLengths res = all_shortest_path_lengths2(graph, 1);
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
      double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
      for (size_t i = 0; i < res.size(); ++i) {
        std::cout << i << "\t\t" << res[i] << "\n";
        if (i > 10) break;
      }
    }

    {
      std::cout << "All shortest paths\n";
      std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
      Graph res = all_shortest_paths(graph, 1);
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
      double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
      // Output path from 2 to 1 
      VertexID id = 2;
      for (size_t i = 0; i < 20; ++i) {
        const Vertex& vertex = res.vertex(id);
        const EdgeList& edges = vertex.edges();
        for (auto p = edges.cbegin(); p != edges.cend(); ++p) {
          std::cout << id << "(" << p->weight() << ") ";
          id = p->dst();
        }
        if (id == 1) break;
      }
      std::cout << "\n";
    }

    {
      std::cout << "All shortest paths V2\n";
      std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
      Graph res = all_shortest_paths2(graph, 1);
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
      double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
      std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
      // Output path from 2 to 1 
      VertexID id = 2;
      for (size_t i = 0; i < 20; ++i) {
        const Vertex& vertex = res.vertex(id);
        const EdgeList& edges = vertex.edges();
        for (auto p = edges.cbegin(); p != edges.cend(); ++p) {
          std::cout << id << "(" << p->weight() << ") ";
          id = p->dst();
        }
        if (id == 1) break;
      }
      std::cout << "\n";
    }

  } 

  if (versie == 2) {

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    Graph2 graph2(true);

    for (unsigned int i = 0; i < size; ++i) {
      graph2.add_vertex(i);
    }
    for (unsigned int i = 0; i < size; ++i) {
      for (unsigned int j = 0; j < nedge; ++j) {
        graph2.add_edge(i, sample(size));
      }
    }
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Generation took " << dif/1000.0 << " seconds." << std::endl;

    t1 = std::chrono::high_resolution_clock::now();
    my_computation(graph2);
    t2 = std::chrono::high_resolution_clock::now();
    dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;

    t1 = std::chrono::high_resolution_clock::now();
    my_parallel_computation2(graph2);
    t2 = std::chrono::high_resolution_clock::now();
    dif = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Computation took " << dif/1000.0 << " seconds." << std::endl;
  }

  return 0;
}

