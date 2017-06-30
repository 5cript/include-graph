#include "graph.hpp"

#include <fstream>
#include <stack>
#include <boost/filesystem.hpp>
#include <boost/graph/graphviz.hpp>

namespace fs = boost::filesystem;

//#####################################################################################################################
struct VertexWriter
{
    VertexWriter(Graph::graph_type* graph)
        : graph_{graph}
    {
    }

    template <typename DescrT>
    void operator()(std::ostream& out, DescrT const& v) const
    {
        out << "[label=\"" << (*graph_)[v].file << "\"]";
    }

private:
    Graph::graph_type* graph_;
};
//#####################################################################################################################
struct EdgeWriter
{
    EdgeWriter(Graph::graph_type* graph)
        : graph_{graph}
    {
    }

    template <typename DescrT>
    void operator()(std::ostream& out, DescrT const& v) const
    {
        //out << "[label=\"" << (*graph_)[v].file << "\"]";
    }

private:
    Graph::graph_type* graph_;
};
//#####################################################################################################################
struct GraphWriter
{
    GraphWriter(Graph::graph_type* graph)
        : graph_{graph}
    {
    }

    void operator()(std::ostream& out) const
    {
        out << "randir = \"LR\";\n";
        //out << "[label=\"" << (*graph_)[v].file << "\"]";
    }

private:
    Graph::graph_type* graph_;
};
//#####################################################################################################################



//#####################################################################################################################
void Graph::loadFromFile(std::string const& file)
{
    std::ifstream reader{file, std::ios_base::binary};
    loadFromStream(reader);
}
//---------------------------------------------------------------------------------------------------------------------
void Graph::loadFromStream(std::istream& stream)
{
    using namespace boost;

    graph_.clear();
    std::string line;

    std::stack <vertex_descriptor> vertexStack;

    // root vertex
    vertexStack.push(add_vertex({"", "ROOT"}, graph_));

    while(std::getline(stream, line))
    {
        if (line.empty())
            return;

        if (line.front() != '.')
            return;

        if (line.back() == '\r')
            line.pop_back();

        int depth = 0;
        while (line.front() == '.')
        {
            ++depth;
            line.erase(0, 1); // drop first character.
        }

        auto p = fs::path{line};
        Vertex v{p.parent_path().string(), p.filename().string()};

        // A) Add vertex
        auto current = add_vertex(v, graph_);

        // B)
        while (vertexStack.size() > depth)
            vertexStack.pop();

        // C)
        add_edge(vertexStack.top(), current, graph_);

        // D)
        vertexStack.push(current);
    }
}
//---------------------------------------------------------------------------------------------------------------------
void Graph::toDotFile(std::string const& fileName)
{
    // write the dot file
    std::ofstream dotfile(fileName.c_str());
    write_graphviz(
        dotfile,
        graph_,
        VertexWriter(&graph_),
        EdgeWriter(&graph_),
        GraphWriter(&graph_)
    );
}
//#####################################################################################################################
