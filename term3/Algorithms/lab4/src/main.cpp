#include "Solution.hpp"
#include "ConsoleColor.hpp"
#include "Benchmark.hpp"

int main() {
    using Color = Ng::Console::Color;

    Ng::Solution<int> solution;
    std::vector<decltype(solution)::Pair> data;

    solution.Rand(100000);
    data = solution.Data();

    std::cout << Color::Green << "Init" << Color::Reset << std::endl;
    std::cout << solution << std::endl;
    std::cout << std::endl;

    std::cout << Color::Blue << "Sort12 (O(n), stable)" << std::endl;
    std::cout << Color::Magenta << Ng::Benchmark([&solution] { solution.Sort12(); }) << " ms" << Color::Reset << std::endl;
//    std::cout << solution << std::endl;
    std::cout << std::endl;

    std::cout << Color::Blue << "Sort13 (O(n), extra memory O(1))" << std::endl;
    solution.Data(data);
    std::cout << Color::Magenta << Ng::Benchmark([&solution] { solution.Sort13(); }) << " ms" << Color::Reset << std::endl;
//    std::cout << solution << std::endl;
    std::cout << std::endl;

    std::cout << Color::Blue << "Sort23 (stable, extra memory O(1))" << Color::Reset << std::endl;
    solution.Data(data);
    std::cout << Color::Magenta << Ng::Benchmark([&solution] { solution.Sort23(); }) << " ms" << Color::Reset << std::endl;
//    std::cout << solution << std::endl;
    std::cout << std::endl;

    return 0;
}
