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

    auto a = add_vertex(g); g[a] = {"A"};
    auto b = add_vertex(g); g[b] = {"B"};
    auto c = add_vertex(g); g[c] = {"C"};
    auto d = add_vertex(g); g[d] = {"D"};
    auto e = add_vertex(g); g[e] = {"E"};

    Graph::edge_descriptor edge;
    bool inserted;
    boost::tie(edge, inserted) = add_edge(a, b, g); if (!inserted) throw; g[edge] = {1};
    boost::tie(edge, inserted) = add_edge(b, c, g); if (!inserted) throw; g[edge] = {2};
    boost::tie(edge, inserted) = add_edge(a, d, g); if (!inserted) throw; g[edge] = {4};
    boost::tie(edge, inserted) = add_edge(b, e, g); if (!inserted) throw; g[edge] = {8};
    boost::tie(edge, inserted) = add_edge(d, e, g); if (!inserted) throw; g[edge] = {16};

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
