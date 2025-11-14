#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/property_map/property_map.hpp>

#include <tinyxml2.h> // Homebrew TinyXML2

using namespace tinyxml2;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " file.graphml\n";
        return 1;
    }

    const std::string filename = argv[1];
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return 1;
    }

    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;
    Graph g;

    boost::dynamic_properties dp;

    // Required: map node_id to vertex_index
    dp.property("node_id", get(boost::vertex_index, g));

    // --- Debug: scan GraphML for all <key> IDs ---
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "Failed to parse GraphML XML\n";
        return 1;
    }

    XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "Invalid GraphML file\n";
        return 1;
    }

    XMLElement* graphElem = root->FirstChildElement("graph");
    if (!graphElem) {
        std::cerr << "No <graph> element found\n";
        return 1;
    }

    // Collect all key IDs
    std::set<std::string> key_ids;
    for (XMLElement* key = root->FirstChildElement("key"); key; key = key->NextSiblingElement("key")) {
        const char* id = key->Attribute("id");
        if (id) key_ids.insert(id);
    }

    std::cout << "Found key IDs in GraphML:\n";
    for (const auto& k : key_ids) std::cout << "  " << k << "\n";

    // Register dummy maps for all keys
    std::map<std::string, std::string> dummy_map;
    for (const auto& k : key_ids) {
        dp.property(k, boost::make_assoc_property_map(dummy_map));
    }

    std::cout << "Registered keys in dynamic_properties.\n";

    // --- Read graphml ---
    try {
        boost::read_graphml(in, g, dp);
    } catch (const std::exception& e) {
        std::cerr << "GraphML parse error (ignored): " << e.what() << "\n";
    }

    // --- Debug output ---
    std::cout << "Vertices: " << num_vertices(g) << "\n";
    std::cout << "Edges: " << num_edges(g) << "\n";

    // Print vertices
    std::cout << "Vertices list:\n";
    for (auto v : boost::make_iterator_range(vertices(g))) {
        std::cout << "v " << v << "\n";
    }

    // Print edges
    std::cout << "Edges list:\n";
    for (auto e : boost::make_iterator_range(edges(g))) {
        std::cout << source(e, g) << " -> " << target(e, g) << "\n";
    }

    return 0;
}
