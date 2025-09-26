#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v{5};

    // Use ranges pipeline to add 1
    auto r = v | std::views::transform([](int x){ return x + 1; });

    for (int n : r) std::cout << n << "\n"; // prints 6
}
