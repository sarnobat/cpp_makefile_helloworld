#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " nodes.csv edges.csv\n";
        return 1;
    }

    std::string nodes_file = argv[1];
    std::string edges_file = argv[2];

    // Map node_id -> label
    std::unordered_map<std::string, std::string> nodes;

    // Read nodes.csv
    std::ifstream nfile(nodes_file);
    if (!nfile) {
        std::cerr << "Cannot open file: " << nodes_file << "\n";
        return 1;
    }

    std::string line;
    // Skip header
    std::getline(nfile, line);

    while (std::getline(nfile, line)) {
        std::stringstream ss(line);
        std::string id, label;

        if (!std::getline(ss, id, ',')) continue;
        if (!std::getline(ss, label)) label = "";

        nodes[id] = label;
    }
    nfile.close();

    // Read edges.csv
    std::vector<std::pair<std::string,std::string>> edges;
    std::ifstream efile(edges_file);
    if (!efile) {
        std::cerr << "Cannot open file: " << edges_file << "\n";
        return 1;
    }

    // Skip header
    std::getline(efile, line);

    while (std::getline(efile, line)) {
        std::stringstream ss(line);
        std::string src, dst;
        if (!std::getline(ss, src, ',')) continue;
        if (!std::getline(ss, dst)) continue;

        edges.emplace_back(src, dst);
    }
    efile.close();

    // Output DOT graph
    std::cout << "digraph G {\n";
    // Nodes
    for (auto &p : nodes) {
        std::string id = p.first;
        std::string label = p.second;
        // Escape quotes in label
        size_t pos = 0;
        while ((pos = label.find('"', pos)) != std::string::npos) {
            label.replace(pos, 1, "\\\"");
            pos += 2;
        }
        std::cout << "    \"" << id << "\" [label=\"" << label << "\"];\n";
    }

    // Edges
    for (auto &e : edges) {
        std::cout << "    \"" << e.first << "\" -> \"" << e.second << "\";\n";
    }

    std::cout << "}\n";

    return 0;
}
