#include "csv.h"
#include <string>
#include <regex>
#include <fstream>
#include <iostream>

// ============================================================================
// ==== READ HEADER
// ============================================================================

struct CSVHeader {
  bool directed;
};


CSVHeader csv_read_header(std::istream& input) {
  CSVHeader header = {false};
  // Read comments 
  std::array<char, 1024> buffer;
  std::regex re_directed("^#[ ]*[Dd]irected[ ]*$");
  std::regex re_undirected("^#[ ]*[Uu]ndirected[ ]*$");
  while (input.peek() == '#') {
    input.getline(&buffer[0], 1024);
    if (input.fail()) throw std::runtime_error("Comment line too long.");
    if (std::regex_match(&buffer[0], re_directed)) header.directed = true;
    if (std::regex_match(&buffer[0], re_undirected)) header.directed = false;
  }
  return header;
}

// ============================================================================
// ==== READ VERTICES
// ============================================================================
//
// A CSV-file with vertices has the following form:
// vertex_id[,vertex_type]
//
Graph& csv_read_vertices(Graph& graph, std::istream& input) {
  std::string id, type;
  int state = 0;
  std::size_t line_number = 0;

  while (input) {
    char c = input.get();
    if (state == 0) {
      // reading the id of the vertex
      if (c >= '0' && c <= '9') {
        id += c;
      } else if (c == ';' || c == ',') {
        state = 1;
      } else if (c == '\n' || '\r' || input.eof()) {
        // type is missing add vertex
        if (id.length() == 0) continue;
        vertex_id id_i = std::stoi(id);
        graph.add_vertex(Vertex(id_i, 0));
        ++line_number;
        id = "";
        type = "";
        state = 0;
      } else {
        throw std::runtime_error("Unexpected character '" + std::to_string(c) + "' at line " + 
          std::to_string(line_number) + ".");
      } 
    } else if (state == 1) {
      // reading the type of the vertex
      if (c >= '0' && c <= '9') {
        type += c;
      } else if (c == '\n' || '\r' || input.eof()) {
        // finished reading type; add vertex
        vertex_id id_i = std::stoi(id);
        vertex_id type_i = std::stoi(type);
        graph.add_vertex(Vertex(id_i, type_i));
        ++line_number;
        id = "";
        type = "";
        state = 0;
      } else {
        throw std::runtime_error("Unexpected character '" + std::to_string(c) + "' at line " + 
          std::to_string(line_number) + ".");
      }
    }
  }
  return graph;
}

// ============================================================================
// ==== READ EDGES
// ============================================================================
//
// Edges have the following form
// src[,dst][,weight][,type]
//


inline void add_edge(Graph& graph, const std::string& src, const std::string& dst, 
    const std::string& weight, const std::string& type, bool add_vertices) {
  vertex_id src_i = std::stoi(src);
  vertex_id dst_i = std::stoi(dst);
  if (add_vertices) {
    graph.add_vertex_if_not_exists(src_i);
    graph.add_vertex_if_not_exists(dst_i);
  }
  edge_weight weight_i = weight.length() == 0 ? 0.0 : std::stof(weight);
  edge_type type_i = type.length() == 0 ? 0 : std::stoi(type);
  graph.add_edge(src_i, dst_i, weight_i, type_i);
}

Graph& csv_read_edges(Graph& graph, std::istream& input, bool add_vertices = true) {
  // Read graph
  std::string src, dst, weight, type;
  int state = 0;
  std::size_t line_number = 0;
  while (input) {
    char c = input.get();
    if (state == 0) {
      // reading src
      if (c >= '0' && c <= '9') {
        src += c;
      } else if (c == ';' || c == ',') {
        state = 1;
      } else if (c == '\n' || '\r' || input.eof()) {
        // empty last line
        if (src.length() == 0) continue;
        throw std::runtime_error("Unexpected end-of-line at line " + 
          std::to_string(line_number) + ".");
      }
    } else if (state == 1) {
      // reading dst
      if (c >= '0' && c <= '9') {
        dst += c;
      } else if (c == ';' || c == ',') {
        state = 2;
      } else if (c == '\n' || '\r' || input.eof()) {
        add_edge(graph, src, dst, weight, type, add_vertices);
        ++line_number;
        src = dst = weight = type = "";
        state = 0;
      } else {
        throw std::runtime_error("Unexpected character '" + std::to_string(c) + "' at line " + 
          std::to_string(line_number) + ".");
      } 
    } else if (state == 2) {
      // reading weight
      if ((c >= '0' && c <= '9') || c == '.') {
        weight += c;
      } else if (c == ';' || c == ',') {
        state = 3;
      } else if (c == '\n' || '\r' || input.eof()) {
        add_edge(graph, src, dst, weight, type, add_vertices);
        ++line_number;
        src = dst = weight = type = "";
        state = 0;
      } else {
        throw std::runtime_error("Unexpected character '" + std::to_string(c) + "' at line " + 
          std::to_string(line_number) + ".");
      }
    } else if (state == 3) {
      // reading type
      if (c >= '0' && c <= '9') {
        type += c;
      } else if (c == '\n' || '\r' || input.eof()) {
        add_edge(graph, src, dst, weight, type, add_vertices);
        ++line_number;
        src = dst = weight = type = "";
        state = 0;
      } else {
        throw std::runtime_error("Unexpected character '" + std::to_string(c) + "' at line " + 
          std::to_string(line_number) + ".");
      }
    }
  }
  return graph;
}


// ============================================================================
// ==== READ GRAPH
// ============================================================================

Graph graph_from_csv(const std::string& vertices_fn, const std::string& edges_fn) {
  std::ifstream edges(edges_fn);
  // TODO if edges.fail()
  CSVHeader header = csv_read_header(edges);
  Graph graph(true);
  if (!header.directed) graph.directed(false);
  std::ifstream vertices(vertices_fn);
  // TODO if vertices.fail()
  csv_read_vertices(graph, vertices);
  csv_read_edges(graph, edges, false);
  return graph;
}


Graph graph_from_csv(const std::string& edges_fn) {
  std::ifstream edges(edges_fn);
  // TODO if edges.fail()
  CSVHeader header = csv_read_header(edges);
  Graph graph(true);
  if (!header.directed) graph.directed(false);
  csv_read_edges(graph, edges, true);
  return graph;
}


// ============================================================================
// ==== WRITE GRAPH
// ============================================================================

void csv_write_vertices_and_edges(const Graph& graph, std::ostream& output_vertices, 
    std::ostream& output_edges, bool write_vertices = true) {

  if (graph.directed()) {
    output_edges << "# Directed\n";
  } else {
    output_edges << "# Undirected\n";
  }

  const VertexList& v = graph.vertices();
  for (auto p = v.begin(); p != v.end(); ++p) {
    if (write_vertices) {
      output_vertices << p->second.id();
      if (p->second.type() != 0) 
        output_vertices << ',' << p->second.type();
      output_vertices << '\n';
    }
    const EdgeList& edges = p->second.edges_out();
    for (auto q = edges.cbegin(); q != edges.cend(); ++q) {
      if (!graph.directed() && (p->first > q->dst())) continue;
      output_edges << p->first << ',' << q->dst();
      if (q->weight() != 1.0) output_edges << ',' << q->weight();
      if (q->type() != 0) output_edges << ',' << q->type();
      output_edges << '\n';
    }
  }
}

void graph_to_csv(const Graph& graph, const std::string& edges_fn) {
  std::ofstream edges(edges_fn);
  csv_write_vertices_and_edges(graph, std::cout, edges, false);
}

void graph_to_csv(const Graph& graph, const std::string& vertices_fn, 
    const std::string& edges_fn) {
  std::ofstream vertices(vertices_fn);
  std::ofstream edges(edges_fn);
  csv_write_vertices_and_edges(graph, vertices, edges, true);
}

