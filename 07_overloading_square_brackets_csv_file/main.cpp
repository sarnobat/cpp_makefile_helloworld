#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

// Data container
struct Data {
    std::vector<std::string> lines;
};

// Source: read from stdin
struct ReadStdin {
    Data operator()() const {
        Data output;
        std::string line;
        while (std::getline(std::cin, line)) {
            output.lines.push_back(line);
        }
        return output;
    }
};

// Transform: convert lines to uppercase
struct ToUpper {
    Data operator()(const Data& input) const {
        Data output = input;
        for (auto& line : output.lines)
            std::transform(line.begin(), line.end(), line.begin(), ::toupper);
        return output;
    }
};

// Filter: select lines matching a predicate
struct Filter {
    std::function<bool(const std::string&)> pred;
    Filter(std::function<bool(const std::string&)> p) : pred(p) {}

    Data operator()(const Data& input) const {
        Data output;
        for (auto& line : input.lines)
            if (pred(line))
                output.lines.push_back(line);
        return output;
    }
};

// Pipe operator
template<typename F, typename G>
auto operator|(F f, G g) {
    return [f, g]() {
        return g(f());
    };
}

int main() {
    auto pipeline = ReadStdin() | ToUpper() | Filter([](const std::string& s){ return s.size() > 3; });
    Data result = pipeline();

    for (auto& line : result.lines)
        std::cout << line << "\n";
}
