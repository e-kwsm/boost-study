#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/format.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/optional/optional.hpp>
#include <boost/property_map/function_property_map.hpp>
#include <boost/range/iterator_range_core.hpp>

struct reach_threshold {};

template<typename EdgeWeight> struct threshold_visitor : boost::default_dijkstra_visitor {
  threshold_visitor(EdgeWeight threshold) : threshold{threshold}, total_weight{0} {}

  template<typename Edge, typename Graph> void examine_edge(Edge e, const Graph& g) {
    min_weight = min_weight ? std::min(*min_weight, g[e].weight) : g[e].weight;
  }
  template<typename Vertex, typename Graph> void finish_vertex(Vertex v, const Graph& g) {
    if (min_weight)
      total_weight += *min_weight;
    if (total_weight >= threshold)
      throw reach_threshold{};
    min_weight = boost::none;
  }

  const EdgeWeight threshold;
  EdgeWeight total_weight;
  boost::optional<EdgeWeight> min_weight;
};

struct Vertex {
  std::string name;
};

std::ostream& operator<<(std::ostream& os, const Vertex& v) {
  os << v.name;
  return os;
}

struct Edge {
  unsigned weight;
};

std::ostream& operator<<(std::ostream& os, const Edge& e) {
  os << e.weight;
  return os;
}

int main() {
  /*
   *      1    4
   *   A----B----C
   *   |    |    |
   *  2|   8|    | 32
   *   |    |    |
   *   D----E----F
   *     16   64
   */

  using Graph = boost::adjacency_list<boost::listS,       // OutEdgeList
                                      boost::vecS,        // VertexList
                                      boost::undirectedS, // Directed
                                      Vertex,             // VertexProperties
                                      Edge                // EdgeProperties
                                      >;
  Graph graph;

  const auto va = boost::add_vertex({"A"}, graph);
  const auto vb = boost::add_vertex({"B"}, graph);
  const auto vc = boost::add_vertex({"C"}, graph);
  const auto vd = boost::add_vertex({"D"}, graph);
  const auto ve = boost::add_vertex({"E"}, graph);
  const auto vf = boost::add_vertex({"F"}, graph);

  boost::add_edge(va, vb, {1}, graph);
  boost::add_edge(va, vd, {2}, graph);
  boost::add_edge(vb, vc, {4}, graph);
  boost::add_edge(vb, ve, {8}, graph);
  boost::add_edge(vd, ve, {16}, graph);
  boost::add_edge(vc, vf, {32}, graph);
  boost::add_edge(ve, vf, {64}, graph);

  std::cout << "# boost::print_graph\n";
  boost::print_graph(graph, boost::get(&Vertex::name, graph));
  std::cout << "# boost::print_vertices\n";
  boost::print_vertices(graph, boost::get(&Vertex::name, graph));
  std::cout << "# iterate over vertices\n";
  for (auto v : boost::make_iterator_range(vertices(graph)))
    std::cout << v << '\t' << graph[v] << '\n';
  std::cout << "# boost::print_edges\n";
  boost::print_edges(graph, boost::get(&Vertex::name, graph));
  std::cout << "# iterate over edges\n";
  for (auto e : boost::make_iterator_range(edges(graph)))
    std::cout << e << '\t' << graph[e] << '\n';

  auto output_distances = [&](Graph::vertex_descriptor src, Graph::vertex_descriptor dest) {
    std::cout << "# boost::dijkstra_shortest_paths\n";
    std::vector<decltype(Edge::weight)> distances(boost::num_vertices(graph));
    std::vector<Graph::vertex_descriptor> predecessors(boost::num_vertices(graph));
    boost::dijkstra_shortest_paths(
        graph, src,
        boost::weight_map(boost::make_function_property_map<Graph::edge_descriptor>(
                              [&](const Graph::edge_descriptor& e) { return graph[e].weight; }))
            .distance_map(&distances[0])
            .predecessor_map(&predecessors[0]));

    for (auto i : boost::make_iterator_range(vertices(graph)))
      std::cout << graph[src].name << " - " << graph[i].name << "\t" << distances[i] << '\n';

    for (auto v = dest; v != src; v = predecessors[v])
      std::cout << graph[v] << " - ";
    std::cout << graph[src] << '\n';
  };

  auto output_distances_threshold = [&](Graph::vertex_descriptor src, unsigned threshold) {
    std::cout << "# boost::dijkstra_shortest_paths w/ custom visitor\n";
    std::vector<decltype(Edge::weight)> distances(boost::num_vertices(graph));
    try {
      boost::dijkstra_shortest_paths(
          graph, src,
          boost::weight_map(boost::make_function_property_map<Graph::edge_descriptor>(
                                [&](const Graph::edge_descriptor& e) { return graph[e].weight; }))
              .distance_map(&distances[0])
              .visitor(threshold_visitor<decltype(Edge::weight)>{threshold}));
    } catch (reach_threshold) {
    }

    for (auto i : boost::make_iterator_range(vertices(graph))) {
      auto d = distances[i];
      if (d == std::numeric_limits<decltype(Edge::weight)>::max())
        std::cout << '#';
      std::cout << graph[src].name << " - " << graph[i].name << "\t" << d << '\n';
    }
  };

  output_distances(va, vf);
  output_distances_threshold(va, 2);

#ifdef GRAPHVIZ
  std::string fname = "graph.dot";
  std::ofstream ofs{fname};
  boost::write_graphviz(ofs, graph, boost::make_label_writer(boost::get(&Vertex::name, graph)));
  ofs.close();
  std::system(std::string{"dot -Tpng -ograph.png " + fname}.c_str());
#endif

  return 0;
}
