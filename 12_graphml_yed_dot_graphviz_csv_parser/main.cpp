#include <tinyxml2.h>
#include <iostream>
#include <string>

using namespace tinyxml2;

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

    std::cout << "source,target\n";

    // Iterate over all <graph> elements
    for (XMLElement* graphElem = root->FirstChildElement("graph"); graphElem; graphElem = graphElem->NextSiblingElement("graph")) {
        for (XMLElement* edgeElem = graphElem->FirstChildElement("edge"); edgeElem; edgeElem = edgeElem->NextSiblingElement("edge")) {
            const char* source = edgeElem->Attribute("source");
            const char* target = edgeElem->Attribute("target");

            if (source && target) {
                std::cout << source << "," << target << "\n";
            }
        }
    }

    return 0;
}
