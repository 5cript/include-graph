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
void Graph::loadFromFile(std::string const& file, std::vector <std::string> const& blacklist)
{
    std::ifstream reader{file, std::ios_base::binary};
    loadFromStream(reader, blacklist);
}
//---------------------------------------------------------------------------------------------------------------------
void Graph::loadFromStream(std::istream& stream, std::vector <std::string> const& blacklist)
{
    using namespace boost;

    graph_.clear();
    std::string line;

    std::stack <vertex_descriptor> vertexStack;

    // root vertex
    vertexStack.push(add_vertex({"", "ROOT"}, graph_));

    int levelIgnore = -1;
    while(std::getline(stream, line))
    {
        if (line.empty())
            return;

        if (line.front() != '.')
            return;

        if (line.back() == '\r')
            line.pop_back();

        unsigned int depth = 0;
        while (line.front() == '.')
        {
            ++depth;
            line.erase(0, 1); // drop first character.
        }

        auto p = fs::path{line};
        Vertex v{p.parent_path().string(), p.filename().string()};

        // black listing
        if (levelIgnore != -1 && depth > levelIgnore)
            continue;
        else
            levelIgnore = -1;
        bool filtered = false;
        for (auto const& b : blacklist)
        {
            auto filterPath = fs::path{b}.make_preferred().string();
            auto prefLine = p.make_preferred().string();
            if (filterPath.empty())
                continue;
            if (filterPath.length() <= prefLine.length() && prefLine.substr(prefLine.length() - filterPath.length(), filterPath.length()) == filterPath)
            {
                filtered = true;
                break;
            }
        }
        if (filtered)
        {
            levelIgnore = depth;

            // add edge, but dont push it to stack, since all following are ignored
            auto current = add_vertex(v, graph_);
            while (vertexStack.size() > depth)
                vertexStack.pop();
            add_edge(vertexStack.top(), current, graph_);
            continue;
        }

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
    std::ofstream stream(fileName.c_str());
    /*
    // write the dot file
    write_graphviz(
        stream,
        graph_,
        VertexWriter(&graph_),
        EdgeWriter(&graph_),
        GraphWriter(&graph_)
    );
    */

    // degreeStack_[0] = all elements with input degree 0

    graph_[0].parent = 0;

    auto es = boost::edges(graph_);
    for (auto eit = es.first; eit != es.second; ++eit)
    {
        graph_[boost::target(*eit, graph_)].parent = boost::source(*eit, graph_);
        graph_[boost::target(*eit, graph_)].descriptor = boost::target(*eit, graph_);
    }

    int maxLevel = 0;
    auto vs = boost::vertices(graph_);
    for (auto vit = vs.first; vit != vs.second; ++vit)
    {
        auto& vertex = graph_[*vit];
        for (auto p = vertex.parent; p != 0; p = graph_[p].parent)
        {
            vertex.level++;
            maxLevel = std::max(maxLevel, vertex.level);
        }
    }
    std::vector <std::vector <Vertex>> levels(maxLevel + 1);

    vs = boost::vertices(graph_);
    for (auto vit = vs.first; vit != vs.second; ++vit)
    {
        levels[maxLevel - graph_[*vit].level].push_back(graph_[*vit]);
    }

    stream << "digraph G\n{\n";
    stream << "\tnode[shape=box];\n";
    stream << "\trankdir=\"LR\";\n";
    stream << "\tsplines=polyline;\n";
    for (auto const& i : levels)
    {
        stream << "\t{\n";
        stream << "\t\trankdir=\"TB\"\n";
        for (auto const& j : i)
        {
            stream << "\t\t" << j.descriptor << "[label=\"" << j.file << "\"];\n";
        }
        stream << "\t}\n";
    }
    stream << "\n";

    es = boost::edges(graph_);
    for (auto eit = es.first; eit != es.second; ++eit)
    {
        stream << "\t" << boost::source(*eit, graph_) << "->" << boost::target(*eit, graph_) << ";\n";
    }
    stream << "}";
}
//#####################################################################################################################
