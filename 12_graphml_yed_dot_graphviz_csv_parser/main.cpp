#include <tinyxml2.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace tinyxml2;

struct Node {
    std::string id;
    std::string label;
};

struct Edge {
    std::string source;
    std::string target;
};

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

    // --- Extract graph-level Description ---
    std::string graph_description;
    for (XMLElement* data = root->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
        const char* key = data->Attribute("key");
        if (key && std::string(key) == "d0") { // d0 = Description
            if (data->GetText())
                graph_description = data->GetText();
            break;
        }
    }

    // --- Find the <graph> element ---
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

        // Check all <data> elements for <y:ShapeNode>/<y:NodeLabel>
        for (XMLElement* data = nodeElem->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            XMLElement* shapeNode = data->FirstChildElement("y:ShapeNode");
            if (!shapeNode) continue;
            XMLElement* nodeLabel = shapeNode->FirstChildElement("y:NodeLabel");
            if (nodeLabel && nodeLabel->GetText()) {
                n.label = nodeLabel->GetText();
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
