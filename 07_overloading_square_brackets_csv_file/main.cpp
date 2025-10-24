#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

struct StdinLines {
    std::vector<std::string> lines;

    StdinLines() {
        std::string line;
        while (std::getline(std::cin, line)) {
            lines.push_back(line);
        }
    }

    const std::string& operator[](size_t n) const {
        if (n >= lines.size()) {
            throw std::out_of_range("Index out of range");
        }
        return lines[n];
    }

    size_t size() const {
        return lines.size();
    }
};

int main() {
    StdinLines stdinLines;

    std::cout << "Total lines read: " << stdinLines.size() << "\n";

    // Example: print the 0th, 1st, 2nd lines if they exist
    for (size_t i = 0; i < stdinLines.size() && i < 3; ++i) {
        std::cout << "Line " << i << ": " << stdinLines[i] << "\n";
    }
}
