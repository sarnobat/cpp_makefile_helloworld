// g++/clang++ -std=c++20 -O2 main.cpp && ./a.out
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

// ---- value-pipeline adaptor: x | then(f) | then(g) ----
template <class F>
struct then_t { F f; };

template <class F>
then_t<F> then(F f) { return {std::move(f)}; }

template <class T, class F>
auto operator|(T&& x, then_t<F> t) -> decltype(auto) {
    return std::invoke(std::move(t.f), std::forward<T>(x));
}

// ---- composition & optional helpers ----
template <class F, class G>
auto compose(F f, G g) {
    return [=](auto&& x) -> decltype(auto) {
        return f(g(std::forward<decltype(x)>(x)));
    };
}

template <class T, class F>
auto and_then(std::optional<T> o, F f) {
    using R = decltype(f(*o));
    return o ? R(f(*o)) : R(std::nullopt);
}

int main() {
    const std::vector<int> data{1,2,3,4,5,6};

    auto is_even   = [](int x){ return x % 2 == 0; };
    auto square    = [](int x){ return x * x; };
    auto to_string = [](int x){ return std::to_string(x); };
    auto shout     = [](std::string s){ return s + ""; };
    auto say_num   = compose(shout, to_string);

    // C++20 ranges pipeline (still uses | with views)
    auto view = data
              | std::views::filter(is_even)
              | std::views::transform(square);

    std::vector<int> squares;
    for (int v : view) squares.push_back(v);

    const int sum = std::accumulate(squares.begin(), squares.end(), 0);

    auto half_if_even = [](int x) -> std::optional<int> {
        return (x % 2 == 0) ? std::optional{x / 2} : std::nullopt;
    };

    // Value pipeline using | then(...)
    auto result_opt = and_then(std::optional{sum}, half_if_even)
                    | then([&](auto opt) -> std::optional<std::size_t> {
                          return opt ? std::optional{squares.size()} : std::nullopt;
                      });

    auto double_it = [](int n){ return n * 2; };
    auto loud_str  = compose(shout, to_string);
    auto demo_str  = 5 | then(double_it) | then(loud_str); // "10!"

    std::cout << "Squares of evens: ";
    for (int v : squares) std::cout << v << " ";
    std::cout << "\nSum: " << sum
              << "\nValue pipe demo: " << demo_str << '\n';

    if (result_opt) std::cout << "Optional flow produced: " << *result_opt << "\n";
    else            std::cout << "Optional flow: no value\n";
}
