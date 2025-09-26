// g++ -std=c++20 -O2 fp_demo.cpp && ./a.out
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

// Minimal value pipe (x |> f) for nice chaining
template <class T, class F>
auto operator|>(T&& x, F&& f) -> decltype(auto) {
    return std::forward<F>(f)(std::forward<T>(x));
}

// Function composition: compose(f, g)(x) == f(g(x))
template <class F, class G>
auto compose(F f, G g) {
    return [=](auto&& x) { return f(g(std::forward<decltype(x)>(x))); };
}

// Optional "and_then" helper (monadic-ish chaining)
template <class T, class F>
auto and_then(std::optional<T> o, F f) {
    using R = decltype(f(*o));
    return o ? R(f(*o)) : R(std::nullopt);
}

int main() {
    // Immutable input
    const std::vector<int> data{1,2,3,4,5,6};

    // Small, pure lambdas
    auto is_even   = [](int x) { return x % 2 == 0; };
    auto square    = [](int x) { return x * x; };
    auto to_string = [](int x) { return std::to_string(x); };
    auto shout     = [](std::string s) { return s + "!"; };

    // Compose two functions (stringify then shout)
    auto say_num = compose(shout, to_string); // say_num(int) -> std::string

    // Ranges pipeline: filter -> transform -> (materialize)
    auto view = data
              | std::views::filter(is_even)
              | std::views::transform(square);

    std::vector<int> squares;
    for (int v : view) squares.push_back(v); // collect without side effects upstream

    // Reduce (fold) to a sum
    const int sum = std::accumulate(squares.begin(), squares.end(), 0);

    // Tiny optional flow: half if even, then stringify+shout via pipeline
    auto half_if_even = [](int x) -> std::optional<int> {
        return (x % 2 == 0) ? std::optional{x / 2} : std::nullopt;
    };

    auto result = and_then(std::optional{sum}, half_if_even)           // Maybe<int>
                |> [&](auto opt) {                                    // higher-order step
                       return opt ? std::optional{squares.size()}      // demo: swap to size
                                  : std::optional<std::size_t>{};
                   };

    // Also show value pipeline with |> (non-range)
    auto double_it = [](int n) { return n * 2; };
    auto loud_str  = compose(shout, to_string);
    auto demo_str  = 5 |> double_it |> loud_str; // "10!"

    std::cout << "Squares of evens: ";
    for (int v : squares) std::cout << v << " ";
    std::cout << "\nSum: " << sum
              << "\nValue pipe demo: " << demo_str << '\n';

    if (result) std::cout << "Optional flow produced: " << *result << "\n";
    else        std::cout << "Optional flow: no value\n";
}
