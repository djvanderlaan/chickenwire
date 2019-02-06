#include "functions.h"
#include <string>
#include <regex>
#include <fstream>

Graph read_from_csv(std::istream& input) {
  Graph graph(true);
  // Read comments 
  std::array<char, 1024> buffer;
  std::regex re_directed("^#[ ]*[Dd]irected[ ]*$");
  std::regex re_undirected("^#[ ]*[Uu]ndirected[ ]*$");
  while (input.peek() == '#') {
    input.getline(&buffer[0], 1024);
    if (input.fail()) throw std::runtime_error("Comment line too long.");
    if (std::regex_match(&buffer[0], re_directed)) graph.directed(true);
    if (std::regex_match(&buffer[0], re_undirected)) graph.directed(false);
  }
  // Read graph
  std::string from, to, weight;
  int state = 0;
  std::size_t line_number = 0;
  while (input) {
    char c = input.get();
    if (state == 0) {
      if (c >= '0' && c <= '9') {
        from += c;
      } else if (c == ';' || c == ',') {
        state = 1;
      } else if (c == '\n' || '\r' || input.eof()) {
        // empty last line
        if (to.length() == 0) continue;
        throw std::runtime_error("Unexpected end-of-line at line " + 
          std::to_string(line_number) + ".");
      }
    } else if (state == 1) {
      if (c >= '0' && c <= '9') {
        to += c;
      } else if (c == ';' || c == ',') {
        state = 2;
      } else if (c == '\n' || '\r' || input.eof()) {
        vertex_id from_i = std::stoi(from);
        vertex_id to_i = std::stoi(to);
        graph.add_vertex_if_not_exists(from_i);
        graph.add_vertex_if_not_exists(to_i);
        graph.add_edge(from_i, to_i);
        ++line_number;
        from = "";
        to = "";
        state = 0;
      }
    } else if (state == 2) {
      if ((c >= '0' && c <= '9') || c == '.') {
        weight += c;
      } else if (c == ';' || c == ',') {
        throw std::runtime_error("Unexpected end-of-field at line " +
          std::to_string(line_number) + ".");
      } else if (c == '\n' || '\r' || input.eof()) {
        vertex_id from_i = std::stoi(from);
        vertex_id to_i = std::stoi(to);
        double weight_i = std::stof(weight);
        graph.add_vertex_if_not_exists(from_i);
        graph.add_vertex_if_not_exists(to_i);
        graph.add_edge(from_i, to_i, weight_i);
        ++line_number;
        from = "";
        to = "";
        weight = "";
        state = 0;
      }
    }
  }
  return graph;
}

Graph read_from_csv(const std::string& filename) {
  std::ifstream file(filename);
  return read_from_csv(file);
}

void write_to_csv(const Graph& graph, std::ostream& output) {
  if (graph.directed()) {
    output << "# Directed\n";
  } else {
    output << "# Undirected\n";
  }
  const VertexList& v = graph.vertices();
  for (auto p = v.begin(); p != v.end(); ++p) {
    const EdgeList& edges = p->second.edges_out();
    for (auto q = edges.cbegin(); q != edges.cend(); ++q) {
      if (p->first >= q->first) continue;
      output << p->first << ';' << q->first;
      if (q->second != 1.0) output << ';' << q->second;
      output << '\n';
    }
  }
}

void write_to_csv(const Graph& graph, const std::string& filename) {
  std::ofstream file(filename);
  write_to_csv(graph, file);
}

