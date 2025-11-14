#include <tinyxml2.h>
#include <iostream>
#include <string>
#include <vector>

using namespace tinyxml2;

struct Node {
    std::string id;
    std::string label; // text from <y:NodeLabel>
};

struct Edge {
    std::string source;
    std::string target;
};

// Recursively find the first NodeLabel element and return its text
std::string findNodeLabel(XMLElement* elem) {
    if (!elem) return "";

    const char* name = elem->Name();
    if (name && std::string(name).find("NodeLabel") != std::string::npos) {
        if (elem->GetText())
            return elem->GetText();
    }

    // Recurse on children
    for (XMLElement* child = elem->FirstChildElement(); child; child = child->NextSiblingElement()) {
        std::string text = findNodeLabel(child);
        if (!text.empty()) return text;
    }

    return "";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " file.graphml\n";
        return 1;
    }

    const char* filename = argv[1];

    XMLDocument doc;
    if (doc.LoadFile(filename) != XML_SUCCESS) {
        std::cerr << "Failed to load XML file: " << filename << "\n";
        return 1;
    }

    XMLElement* root = doc.RootElement(); // <graphml>
    if (!root) {
        std::cerr << "Invalid GraphML file\n";
        return 1;
    }

    // --- Graph-level description ---
    std::string graph_description;
    for (XMLElement* data = root->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
        const char* key = data->Attribute("key");
        if (key && std::string(key) == "d0" && data->GetText()) {
            graph_description = data->GetText();
            break;
        }
    }

    // --- Find <graph> element ---
    XMLElement* graphElem = root->FirstChildElement("graph");
    if (!graphElem) {
        std::cerr << "No <graph> element found\n";
        return 1;
    }

    // --- Parse nodes ---
    std::vector<Node> nodes;
    for (XMLElement* nodeElem = graphElem->FirstChildElement("node"); nodeElem; nodeElem = nodeElem->NextSiblingElement("node")) {
        Node n;
        const char* id = nodeElem->Attribute("id");
        if (!id) continue;
        n.id = id;
        n.label = "";

        // Iterate over all <data> elements
        for (XMLElement* data = nodeElem->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            std::string text = findNodeLabel(data);
            if (!text.empty()) {
                n.label = text;
                break; // take first label found
            }
        }

        nodes.push_back(n);
    }

    // --- Parse edges ---
    std::vector<Edge> edges;
    for (XMLElement* edgeElem = graphElem->FirstChildElement("edge"); edgeElem; edgeElem = edgeElem->NextSiblingElement("edge")) {
        Edge e;
        const char* src = edgeElem->Attribute("source");
        const char* tgt = edgeElem->Attribute("target");
        if (!src || !tgt) continue;
        e.source = src;
        e.target = tgt;
        edges.push_back(e);
    }

    // --- Output ---
    std::cout << "Graph Description: " << (graph_description.empty() ? "(none)" : graph_description) << "\n\n";

    std::cout << "Vertices (" << nodes.size() << "):\n";
    for (const auto& n : nodes) {
        std::cout << n.id;
        if (!n.label.empty()) std::cout << " : " << n.label;
        std::cout << "\n";
    }

    std::cout << "\nEdges (" << edges.size() << "):\n";
    for (const auto& e : edges) {
        std::cout << e.source << " -> " << e.target << "\n";
    }

    return 0;
}
