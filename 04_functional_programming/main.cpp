#include <iostream>
#include <ranges>
 
int main()
{
    auto const ints = {0, 1, 2, 3, 4, 5};
    auto even = [](int i) { return 0 == i % 2; };
    auto square = [](int i) { return i * i; };
 
	///
    /// the "pipe" syntax of composing the views:
	/// this ranges library was introduced in C++20
	///
    for (int i : ints | std::views::filter(even) | std::views::transform(square))
        std::cout << i << ' ';
 
	for (int i :
		std::ranges::operator|(
			std::ranges::operator|(
				ints,
				std::views::filter(even)),
			std::views::transform(square)))
	{
		std::cout << i << ' ';
	}


    std::cout << '\n';

	///
    /// a traditional "functional" composing syntax:
	///
    for (int i : std::views::transform(std::views::filter(ints, even), square))
        std::cout << i << ' ';
}
