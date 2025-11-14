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
void printNodeLabels(XMLElement* elem, const std::string& node_id) {
    if (!elem) return;

    const char* name = elem->Name();
    if (name && std::string(name).find("NodeLabel") != std::string::npos) {
        std::string text = extractAllText(elem);
        if (!text.empty()) {
            std::cout << node_id << "," << text << "\n";
        }
    }

    for (XMLNode* child = elem->FirstChild(); child; child = child->NextSibling()) {
        if (XMLElement* childElem = child->ToElement()) {
            printNodeLabels(childElem, node_id);
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

    XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "Invalid GraphML file\n";
        return 1;
    }

    std::cout << "node_id,label\n";

    for (XMLElement* graphElem = root->FirstChildElement("graph"); graphElem; graphElem = graphElem->NextSiblingElement("graph")) {
        for (XMLElement* nodeElem = graphElem->FirstChildElement("node"); nodeElem; nodeElem = nodeElem->NextSiblingElement("node")) {
            const char* id = nodeElem->Attribute("id");
            if (!id) continue;
            printNodeLabels(nodeElem, id); // print each label individually
        }
    }

    return 0;
}
