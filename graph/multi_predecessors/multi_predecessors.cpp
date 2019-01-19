#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphviz.hpp>

template <typename Edge, typename PredecessorsMap>
class multi_predecessors_map : public boost::default_bfs_visitor {
 public:
  multi_predecessors_map(PredecessorsMap predecessors_map)
      : predecessors_map{predecessors_map} {}

  template <typename Graph>
  void examine_edge(Edge e, const Graph& g) {
    edges.push_back(e);
  }

  template <typename Graph>
  void black_target(Edge e, const Graph& g) {
    edges.erase(std::remove_if(edges.begin(), edges.end(),
                               [=](Edge e2) { return e == e2; }),
                edges.end());
  }

  template <typename Vertex, typename Graph>
  void finish_vertex(Vertex v, const Graph& g) {
    for (auto e : edges)
      predecessors_map->insert({boost::target(e, g), boost::source(e, g)});
    edges.clear();
  }

 protected:
  PredecessorsMap predecessors_map;
  std::vector<Edge> edges;
};

template <typename Edge, typename PredecessorsMap, typename DistanceMap>
class multi_predecessors_map_w_distances
    : public multi_predecessors_map<Edge, PredecessorsMap> {
 public:
  multi_predecessors_map_w_distances(PredecessorsMap predecessors_map,
                                     DistanceMap distance_map)
      : multi_predecessors_map<Edge, PredecessorsMap>{predecessors_map},
        distance_map{distance_map} {}

  template <typename Graph>
  void tree_edge(Edge e, const Graph& g) {
    distance_map[boost::target(e, g)] = distance_map[boost::source(e, g)] + 1;
  }

 private:
  DistanceMap distance_map;
};

int main() {
  /*
   *     +---a---+
   *     |   |   |
   * +---b   c   d
   * |   |   |   |
   * e   f   g   h
   * |    \ /    |
   * +-----i-----+
   *
   */

  using Graph = boost::adjacency_list<
      boost::listS,                                       // OutEdgeList
      boost::vecS,                                        // VertexList
      boost::undirectedS,                                 // Directed
      boost::property<boost::vertex_name_t, std::string>  // VertexProperties
      >;
  Graph graph;

  const auto va = boost::add_vertex({"A"}, graph);
  const auto vb = boost::add_vertex({"B"}, graph);
  const auto vc = boost::add_vertex({"C"}, graph);
  const auto vd = boost::add_vertex({"D"}, graph);
  const auto ve = boost::add_vertex({"E"}, graph);
  const auto vf = boost::add_vertex({"F"}, graph);
  const auto vg = boost::add_vertex({"G"}, graph);
  const auto vh = boost::add_vertex({"H"}, graph);
  const auto vi = boost::add_vertex({"I"}, graph);

  boost::add_edge(va, vb, graph);
  boost::add_edge(va, vc, graph);
  boost::add_edge(va, vd, graph);
  boost::add_edge(vb, ve, graph);
  boost::add_edge(vb, vf, graph);
  boost::add_edge(vc, vg, graph);
  boost::add_edge(vd, vh, graph);
  boost::add_edge(ve, vi, graph);
  boost::add_edge(vf, vi, graph);
  boost::add_edge(vg, vi, graph);
  boost::add_edge(vh, vi, graph);

#ifdef GRAPHVIZ
  std::string fname = "graph.dot";
  std::ofstream ofs{fname};
  boost::write_graphviz(
      ofs, graph,
      boost::make_label_writer(boost::get(boost::vertex_name, graph)));
  ofs.close();
  std::system(std::string{"dot -Tpng -ograph.png " + fname}.c_str());
#endif

  auto output_distances = [&](Graph::vertex_descriptor src) {
    std::multimap<Graph::vertex_descriptor, Graph::vertex_descriptor>
        predecessors;
    std::vector<unsigned> distances(boost::num_vertices(graph));

    boost::breadth_first_search(
        graph, src,
        boost::visitor(multi_predecessors_map_w_distances<
                       Graph::edge_descriptor, decltype(&predecessors),
                       decltype(&distances[0])>{&predecessors, &distances[0]}));

    std::cout << "#distance from " << boost::get(boost::vertex_name, graph, src)
              << "\n";
    for (auto v : boost::make_iterator_range(vertices(graph)))
      std::cout << boost::get(boost::vertex_name, graph, v) << "    "
                << distances[v] << "\n";
    std::cout << "#predecessor\n";
    for (auto [s, t] : predecessors)
      std::cout << boost::get(boost::vertex_name, graph, s) << " -> "
                << boost::get(boost::vertex_name, graph, t) << "\n";
  };

  output_distances(vi);

  return 0;
}
