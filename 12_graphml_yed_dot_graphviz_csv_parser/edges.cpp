#include <tinyxml2.h>
#include <iostream>
#include <string>
#include <getopt.h>

using namespace tinyxml2;

std::string leafID(const std::string& id, bool short_ids) {
    if (!short_ids) return id;
    auto pos = id.rfind("::");
    if (pos != std::string::npos) return id.substr(pos + 2);
    return id;
}

int main(int argc, char** argv) {
    bool short_ids = false;

    // Option parsing
    static struct option long_options[] = {
        {"short", no_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "s", long_options, &option_index)) != -1) {
        switch (opt) {
            case 's': short_ids = true; break;
            default:
                std::cerr << "Usage: " << argv[0] << " [--short] file.graphml\n";
                return 1;
        }
    }

    if (optind >= argc) {
        std::cerr << "Missing GraphML file argument\n";
        return 1;
    }

    std::string filename = argv[optind];

    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "Failed to load XML file\n";
        return 1;
    }

    XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "Invalid GraphML file\n";
        return 1;
    }

    std::cout << "source,target\n";

    for (XMLElement* graphElem = root->FirstChildElement("graph"); graphElem; graphElem = graphElem->NextSiblingElement("graph")) {
        for (XMLElement* edgeElem = graphElem->FirstChildElement("edge"); edgeElem; edgeElem = edgeElem->NextSiblingElement("edge")) {
            const char* source = edgeElem->Attribute("source");
            const char* target = edgeElem->Attribute("target");
            if (!source || !target) continue;

            std::cout << leafID(source, short_ids) << "," << leafID(target, short_ids) << "\n";
        }
    }

    return 0;
}
