#include "graph.hpp"
#include "stl_headers.hpp"

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("dot,d", po::value <std::string>(), "path to graphviz dot command")
        ("format,f", po::value <std::string>(), "output file format (eg. png)")
        ("output,o", po::value <std::string>(), "output file name")
        ("input,i", po::value <std::string>(), "output file name")
        ("blacklist,b", po::value <std::string>(), "file name blacklist, such as \"iostream,asio.hpp,cmath\"")
        ("ignore-stl,s", "ignore stl headers")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    std::string dot = "dot";
    std::string format = "png";
    std::string output = "./graph.png";
    std::string input = "./dependencyGraph.txt";
    std::vector <std::string> blacklist = {};

    if (vm.count("dot"))
    {
        dot = vm["dot"].as <std::string>();
    }
    if (vm.count("format"))
    {
        format = vm["format"].as <std::string>();
    }
    if (vm.count("output"))
    {
        output = vm["output"].as <std::string>();
    }
    if (vm.count("input"))
    {
        input = vm["input"].as <std::string>();
    }
    if (vm.count("blacklist"))
    {
        auto blacklistList = vm["blacklist"].as <std::string>();
        boost::split(blacklist, blacklistList, boost::is_any_of(","));
    }
    if (vm.count("ignore-stl"))
    {
        addStlHeaders(blacklist);
    }

    Graph graph;
    graph.loadFromFile(input, blacklist);
    graph.toDotFile("graph.dot");

    std::cout << "please wait, rendering takes time.\n";
    system((dot + " -T" + format + " ./graph.dot -o " + output).c_str());

    return 0;
}
