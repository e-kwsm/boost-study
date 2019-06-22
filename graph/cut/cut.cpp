#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>
#include <boost/property_map/property_map.hpp>

int main() {
  /*
   *    A
   *  / | \
   * B--C--D
   *  \   /
   *    E
   *  /   \
   * F--G--H
   *  \ | /
   *    I
   */

  boost::adjacency_list<boost::listS,                                        // OutEdgeList
                        boost::vecS,                                         // VertexList
                        boost::undirectedS,                                  // Directed
                        boost::property<boost::vertex_name_t, std::string>,  // VertexProperties
                        boost::property<boost::edge_weight_t, unsigned>      // EdgeProperties
                        >
      graph;

  const auto va = boost::add_vertex({"A"}, graph);
  const auto vb = boost::add_vertex({"B"}, graph);
  const auto vc = boost::add_vertex({"C"}, graph);
  const auto vd = boost::add_vertex({"D"}, graph);
  const auto ve = boost::add_vertex({"E"}, graph);
  const auto vf = boost::add_vertex({"F"}, graph);
  const auto vg = boost::add_vertex({"G"}, graph);
  const auto vh = boost::add_vertex({"H"}, graph);
  const auto vi = boost::add_vertex({"I"}, graph);

  boost::add_edge(va, vb, {1}, graph);
  boost::add_edge(va, vc, {1}, graph);
  boost::add_edge(va, vd, {1}, graph);
  boost::add_edge(vb, vc, {1}, graph);
  boost::add_edge(vc, vd, {1}, graph);
  boost::add_edge(vb, ve, {1}, graph);
  boost::add_edge(vd, ve, {1}, graph);
  boost::add_edge(ve, vf, {1}, graph);
  boost::add_edge(ve, vh, {1}, graph);
  boost::add_edge(vf, vg, {1}, graph);
  boost::add_edge(vg, vh, {1}, graph);
  boost::add_edge(vf, vi, {1}, graph);
  boost::add_edge(vg, vi, {1}, graph);
  boost::add_edge(vh, vi, {1}, graph);

  auto parities = boost::make_one_bit_color_map(boost::num_vertices(graph),
                                                boost::get(boost::vertex_index, graph));
  const auto w = boost::stoer_wagner_min_cut(graph, boost::get(boost::edge_weight, graph),
                                             boost::parity_map(parities));
  std::cout << "cut weight: " << w << '\n';

  for (size_t i = 0; i < boost::num_vertices(graph); i++) {
    if (boost::get(parities, i))
      std::cout << "one side\t" << boost::get(boost::vertex_name, graph, i) << '\n';
    else
      std::cout << "the other\t" << boost::get(boost::vertex_name, graph, i) << '\n';
  }

#ifdef GRAPHVIZ
  std::string fname = "graph.dot";
  std::ofstream ofs{fname};
  boost::write_graphviz(ofs, graph,
                        boost::make_label_writer(boost::get(boost::vertex_name, graph)));
  ofs.close();
  std::system(std::string{"dot -Tpng -ograph.png " + fname}.c_str());
#endif

  return 0;
}
