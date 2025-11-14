#include <tinyxml2.h>
#include <iostream>
#include <string>

using namespace tinyxml2;

// Recursively extract all text inside an XMLNode
std::string extractAllText(XMLNode* node) {
    if (!node) return "";

    std::string text;
    for (XMLNode* child = node->FirstChild(); child; child = child->NextSibling()) {
        if (child->ToText()) {
            text += child->Value();
        } else {
            text += extractAllText(child);
        }
    }
    return text;
}

// Recursively find all NodeLabel elements under an element
void collectNodeLabels(XMLElement* elem) {
    if (!elem) return;

    const char* name = elem->Name();
    if (name && std::string(name).find("NodeLabel") != std::string::npos) {
        std::string text = extractAllText(elem);
        if (!text.empty()) {
            std::cout << "NodeLabel: " << text << "\n";
        }
    }

    // Recurse into **all children**, not just first-level
    for (XMLNode* child = elem->FirstChild(); child; child = child->NextSibling()) {
        if (XMLElement* childElem = child->ToElement()) {
            collectNodeLabels(childElem);
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " file.graphml\n";
        return 1;
    }

    XMLDocument doc;
    if (doc.LoadFile(argv[1]) != XML_SUCCESS) {
        std::cerr << "Failed to load XML file\n";
        return 1;
    }

    XMLElement* root = doc.RootElement(); // <graphml>
    if (!root) {
        std::cerr << "Invalid GraphML file\n";
        return 1;
    }

    // Iterate all <node> elements under <graph>
    for (XMLElement* graphElem = root->FirstChildElement("graph"); graphElem; graphElem = graphElem->NextSiblingElement("graph")) {
        for (XMLElement* nodeElem = graphElem->FirstChildElement("node"); nodeElem; nodeElem = nodeElem->NextSiblingElement("node")) {
            collectNodeLabels(nodeElem); // fully recurse inside the node
        }
    }

    return 0;
}
