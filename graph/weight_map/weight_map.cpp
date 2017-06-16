#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <boost/format.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graphviz.hpp>
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

int main()
{
    /*
     *      1    2
     *   A----B----C
     *   |    |
     *  4|   8|
     *   |    |
     *   D----E
     *     16
     */

    Graph g;

    const auto a = add_vertex({"A"}, g);
    const auto b = add_vertex({"B"}, g);
    const auto c = add_vertex({"C"}, g);
    const auto d = add_vertex({"D"}, g);
    const auto e = add_vertex({"E"}, g);

    add_edge(a, b, {1},  g);
    add_edge(b, c, {2},  g);
    add_edge(a, d, {4},  g);
    add_edge(b, e, {8},  g);
    add_edge(d, e, {16}, g);

    std::cout << "# boost::print_graph\n";
    boost::print_graph(g, get(&Vertex::name, g));
    std::cout << "# boost::print_vertices\n";
    boost::print_vertices(g, get(&Vertex::name, g));
    std::cout << "# iterate over vertices\n";
    for (auto v : boost::make_iterator_range(vertices(g)))
        std::cout << v << '\t' << g[v] << '\n';
    std::cout << "# boost::print_edges\n";
    boost::print_edges(g, get(&Vertex::name, g));
    std::cout << "# iterate over edges\n";
    for (auto e : boost::make_iterator_range(edges(g)))
        std::cout << e << '\t' << g[e] << '\n';

    auto output_distances = [&](Graph::vertex_descriptor v)
    {
        std::cout << "# boost::dijkstra_shortest_paths\n";
        std::vector<decltype(Edge::weight)> distances(boost::num_vertices(g));
        boost::dijkstra_shortest_paths(g, v,
                boost::weight_map(
                    boost::make_function_property_map<Graph::edge_descriptor>(
                        [&](const Graph::edge_descriptor& e)->decltype(Edge::weight)
                        { return g[e].weight; })
                    ).distance_map(&distances[0]));

        for (auto i : boost::make_iterator_range(vertices(g)))
        {
            std::cout << g[v].name << " - " << g[i].name << "\t" << distances[i] << '\n';
        }
    };

    output_distances(a);

#ifdef GRAPHVIZ
    std::string fname = "graph.dot";
    std::ofstream ofs{fname};
    boost::write_graphviz(ofs, g, boost::make_label_writer(get(&Vertex::name, g)));
    ofs.close();
    std::system(std::string{"dot -Tpng -ograph.png " + fname}.c_str());
#endif

    return 0;
}
