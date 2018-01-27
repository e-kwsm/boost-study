#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <boost/format.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/optional/optional.hpp>
#include <boost/property_map/function_property_map.hpp>
#include <boost/range/iterator_range_core.hpp>

struct Vertex
{
    std::string name;
};

std::ostream& operator<<(std::ostream& os, const Vertex& v)
{
    os << v.name;
    return os;
}

struct Edge
{
    unsigned weight;
};

std::ostream& operator<<(std::ostream& os, const Edge& e)
{
    os << e.weight;
    return os;
}

using Graph = boost::adjacency_list<
    boost::listS,       // OutEdgeList
    boost::vecS,        // VertexList
    boost::undirectedS, // Directed
    Vertex,             // VertexProperties
    Edge,               // EdgeProperties
    boost::no_property, // GraphProperties
    boost::listS        // EdgeList
    >;

struct reach_threshold {};

struct threshold_visitor : boost::default_dijkstra_visitor
{
   threshold_visitor(decltype(Edge::weight) threshold): threshold{threshold}, total_weight{0} {}

   void examine_edge(Graph::edge_descriptor e, const Graph& g)
   {
       min_weight = min_weight ? std::min(*min_weight, g[e].weight) : g[e].weight;
   }
   void finish_vertex(Graph::vertex_descriptor v, const Graph& g)
   {
       if (min_weight) total_weight += *min_weight;
       if (total_weight >= threshold) throw reach_threshold{};
       min_weight = boost::none;
   }

   const decltype(Edge::weight) threshold;
   decltype(Edge::weight) total_weight;
   boost::optional<decltype(Edge::weight)> min_weight;
};

int main()
{
    /*
     *      1    4
     *   A----B----C
     *   |    |    |
     *  2|   8|    | 32
     *   |    |    |
     *   D----E----F
     *     16   64
     */

    Graph graph;

    const auto va = add_vertex({"A"}, graph);
    const auto vb = add_vertex({"B"}, graph);
    const auto vc = add_vertex({"C"}, graph);
    const auto vd = add_vertex({"D"}, graph);
    const auto ve = add_vertex({"E"}, graph);
    const auto vf = add_vertex({"F"}, graph);

    add_edge(va, vb, {1},  graph);
    add_edge(va, vd, {2},  graph);
    add_edge(vb, vc, {4},  graph);
    add_edge(vb, ve, {8},  graph);
    add_edge(vd, ve, {16}, graph);
    add_edge(vc, vf, {32}, graph);
    add_edge(ve, vf, {64}, graph);

    std::cout << "# boost::print_graph\n";
    boost::print_graph(graph, get(&Vertex::name, graph));
    std::cout << "# boost::print_vertices\n";
    boost::print_vertices(graph, get(&Vertex::name, graph));
    std::cout << "# iterate over vertices\n";
    for (auto v : boost::make_iterator_range(vertices(graph)))
        std::cout << v << '\t' << graph[v] << '\n';
    std::cout << "# boost::print_edges\n";
    boost::print_edges(graph, get(&Vertex::name, graph));
    std::cout << "# iterate over edges\n";
    for (auto e : boost::make_iterator_range(edges(graph)))
        std::cout << e << '\t' << graph[e] << '\n';

    auto output_distances = [&](Graph::vertex_descriptor v)
    {
        std::cout << "# boost::dijkstra_shortest_paths\n";
        std::vector<decltype(Edge::weight)> distances(boost::num_vertices(graph));
        boost::dijkstra_shortest_paths(graph, v,
                boost::weight_map(
                    boost::make_function_property_map<Graph::edge_descriptor>(
                        [&](const Graph::edge_descriptor& e)->decltype(Edge::weight)
                        { return graph[e].weight; })
                    ).distance_map(&distances[0]));

        for (auto i : boost::make_iterator_range(vertices(graph)))
        {
            std::cout << graph[v].name << " - " << graph[i].name << "\t" << distances[i] << '\n';
        }
    };

    auto output_distances_threshold = [&](Graph::vertex_descriptor v, unsigned threshold)
    {
        std::cout << "# boost::dijkstra_shortest_paths w/ custom visitor\n";
        std::vector<decltype(Edge::weight)> distances(boost::num_vertices(graph));
        try
        {
            boost::dijkstra_shortest_paths(graph, v,
                    boost::weight_map(
                        boost::make_function_property_map<Graph::edge_descriptor>(
                            [&](const Graph::edge_descriptor& e)->decltype(Edge::weight)
                            { return graph[e].weight; }
                            )
                        ).distance_map(&distances[0])
                    .visitor(threshold_visitor{threshold}));
        }
        catch (reach_threshold)
        {}

        for (auto i : boost::make_iterator_range(vertices(graph)))
        {
            auto d = distances[i];
            if (d == std::numeric_limits<decltype(Edge::weight)>::max())
                std::cout << '#';
            std::cout << graph[v].name << " - " << graph[i].name << "\t" << d << '\n';
        }
    };

    output_distances(va);
    output_distances_threshold(va, 2);

#ifdef GRAPHVIZ
    std::string fname = "graph.dot";
    std::ofstream ofs{fname};
    boost::write_graphviz(ofs, graph, boost::make_label_writer(get(&Vertex::name, graph)));
    ofs.close();
    std::system(std::string{"dot -Tpng -ograph.png " + fname}.c_str());
#endif

    return 0;
}
