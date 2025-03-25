#include <algorithm>
#include <boost/type_index.hpp>
#include <concepts>
#include <execution>
#include <iostream>
#include <list>
#include <print>
#include <random>
#include <ranges>
#include <typeinfo>
#include <vector>

namespace partitioning {
using boost::typeindex::type_id_with_cvr;
using std::cout;
using std::endl;
using std::vector;
using namespace std::ranges;

template <std::ranges::forward_range Rng>
auto stringify(Rng&& seq) {
    auto b = seq |
             views::transform([](const auto& a) { return std::to_string(a); }) |
             views::join_with(',') | views::common;
    return "{" + std::string(std::begin(b), std::end(b)) + "}";
}

template <std::ranges::forward_range Rng, typename Func>
auto stringify(Rng&& seq, Func&& func) {
    auto b = seq |
             views::transform([&func](const auto& a) { return func(a); }) |
             views::join_with(',') | views::common;
    return "{" + std::string(std::begin(b), std::end(b)) + "}";
}

void main() {
    // Pode-se entender o particionamento como uma operação de ordenamento em
    // função de algum predicado booleano. os elementos cujos teste retornam
    // 'verdadeiro' ficam então agrupados e separados dos elementos que retornam
    // 'falso'.

    cout << endl;
    cout << "std::ranges::partition(v, [t=5](auto& a) { return a < t; })"
         << endl;
    vector<int> v1 = std::views::iota(1, 9) | std::ranges::to<vector<int>>();
    std::ranges::shuffle(v1, std::random_device{});
    cout << "'v': " << stringify(v1) << endl;
    auto tail1 = std::ranges::partition(v1, [t = 5](auto& a) { return a < t; });
    cout << "x < 5: "
         << stringify(std::ranges::subrange(std::begin(v1), std::begin(tail1)))
         << endl;
    cout << "x >= 5: " << stringify(tail1) << endl;

    cout << endl;
    cout << "std::ranges::stable_partition(v, [t=5](auto& a) { return a < t; })"
         << endl;
    vector<int> v2 = std::views::iota(1, 9) | std::ranges::to<vector<int>>();
    std::ranges::shuffle(v2, std::random_device{});
    cout << "'v': " << stringify(v2) << endl;
    auto tail2 =
        std::ranges::stable_partition(v2, [t = 5](auto& a) { return a < t; });
    cout << "x < 5: "
         << stringify(std::ranges::subrange(std::begin(v2), std::begin(tail2)))
         << endl;
    cout << "x >= 5: " << stringify(tail2) << endl;

    cout << endl;
    cout << "std::ranges::is_partitioned(v, [](auto& a) { return a % 2 == 0; })"
         << endl;
    vector<int> v3 = std::views::iota(1, 9) | std::ranges::to<vector<int>>();
    std::ranges::shuffle(v3, std::random_device{});
    std::ranges::partition(v3, [](auto& a) { return a % 2 == 0; });
    cout << "'v': " << stringify(v3) << endl;
    auto cmp1 =
        std::ranges::is_partitioned(v3, [](auto& a) { return a % 2 == 0; });
    cout << "is 'v' partitioned? " << std::boolalpha << cmp1 << endl;

    cout << endl;
    cout << "std::ranges::partition_copy(v, std::back_inserter(even), "
            "std::back_inserter(odd), [](auto& a) { return a % 2 == 0; })"
         << endl;
    vector<int> v4 = std::views::iota(1, 9) | std::ranges::to<vector<int>>();
    std::ranges::shuffle(v4, std::random_device{});
    vector<int> even;
    vector<int> odd;
    std::ranges::partition_copy(v4, std::back_inserter(even),
                                std::back_inserter(odd),
                                [](auto& a) { return a % 2 == 0; });
    cout << "'v': " << stringify(v4) << endl;
    cout << "'even': " << stringify(even) << endl;
    cout << "'odd': " << stringify(odd) << endl;

    cout << endl;
    cout << "std::ranges::nth_element(v, v.begin() + 4, std::greater<>{}):"
         << endl;
    vector<int> v5 = std::views::iota(1, 9) | std::ranges::to<vector<int>>();
    std::ranges::shuffle(v5, std::random_device{});
    cout << "unsorted 'v': " << stringify(v5) << endl;
    std::ranges::nth_element(v5, std::next(std::begin(v5), 4),
                             std::greater<>{});
    cout << "partially sorted 'v': " << stringify(v5) << endl;
};
}  // namespace partitioning
