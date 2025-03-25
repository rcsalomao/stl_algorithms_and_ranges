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

namespace divide_and_conquer {
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

struct S {
    int value;
    char label;

    // bool operator<(const S& other) const { return value < other.value; }
    friend bool operator==(const S& s1, const S& s2) {
        return s1.value == s2.value;
    }
    friend auto operator<=>(const S& s1, const S& s2) {
        return s1.value <=> s2.value;
    }
    friend std::ostream& operator<<(std::ostream& os, S& o) {
        return os << '{' << o.value << ", " << o.label << '}';
    }
};

struct Cmp {
    bool operator()(const S& s, int i) const { return s.value < i; }
    bool operator()(int i, const S& s) const { return i < s.value; }
};

void main() {
    cout << endl;
    cout << "lb = std::ranges::lower_bound(v, 4); ub = "
            "std::ranges::upper_bound(v, 6);"
         << endl;
    vector<int> v1 = std::views::iota(1, 9) | std::ranges::to<vector<int>>();
    cout << "'v': " << stringify(v1) << endl;
    auto lb =
        std::ranges::lower_bound(v1, 4);  // primeiro elemento em que *it >= 4
    auto ub =
        std::ranges::upper_bound(v1, 6);  // primeiro elemento em que 6 < *it
    cout << "[lb, ub): [" << *lb << ", " << *ub << ")" << endl;
    cout << "lower range: "
         << stringify(std::ranges::subrange(std::begin(v1), lb)) << endl;
    cout << "middle range: " << stringify(std::ranges::subrange(lb, ub))
         << endl;
    cout << "upper range: "
         << stringify(std::ranges::subrange(ub, std::end(v1))) << endl;

    cout << endl;
    cout << "auto [lb, ub] = std::ranges::equal_range(v, 5);" << endl;
    vector<int> v2 = std::views::iota(1, 9) | std::ranges::to<vector<int>>();
    cout << "'v': " << stringify(v2) << endl;
    auto [lb2, ub2] = std::ranges::equal_range(v2, 5);
    cout << "[lb, ub): [" << *lb2 << ", " << *ub2 << ")" << endl;
    cout << "lower range: "
         << stringify(std::ranges::subrange(std::begin(v2), lb2)) << endl;
    cout << "middle range: " << stringify(std::ranges::subrange(lb2, ub2))
         << endl;
    cout << "upper range: "
         << stringify(std::ranges::subrange(ub2, std::end(v2))) << endl;

    cout << endl;
    std::cout
        << "vector<S> v{...}; S val{...}; std::ranges::equal_range(v, val): "
        << endl;
    std::vector<S> v3{{1, 'A'}, {2, 'B'}, {2, 'C'},
                      {2, 'D'}, {3, 'F'}, {4, 'G'}};
    const S val{2, '?'};
    // auto [lb3, ub3] = std::equal_range(
    //     v3.begin(), v3.end(), val);  // basta apenas definir o operador '<'.
    auto [lb3, ub3] = std::ranges::equal_range(v3, val);
    cout << "[lb, ub): [" << *lb3 << ", " << *ub3 << ")" << endl;

    cout << endl;
    std::cout
        << "vector<S> v{...}; std::equal_range(v.begin(), v.end(), 3, Cmp{}): "
        << endl;
    std::vector<S> v4{{1, 'A'}, {2, 'B'}, {2, 'C'},
                      {2, 'D'}, {3, 'F'}, {4, 'G'}};
    auto [lb4, ub4] = std::equal_range(v4.begin(), v4.end(), 3, Cmp{});
    cout << "[lb, ub): [" << *lb4 << ", " << *ub4 << ")" << endl;

    cout << endl;
    std::cout << "vector<S> v{...}; std::ranges::equal_range(v, 2, {}, [](S& "
                 "s) { return s.value; }): "
              << endl;
    std::vector<S> v5{{1, 'A'}, {2, 'B'}, {2, 'C'},
                      {2, 'D'}, {3, 'F'}, {4, 'G'}};
    auto [lb5, ub5] =
        std::ranges::equal_range(v5, 2, {}, [](S& s) { return s.value; });
    cout << "[lb, ub): [" << *lb5 << ", " << *ub5 << ")" << endl;

    cout << endl;
    std::cout
        << "std::ranges::partition_point(v, [](int& i) { return i < 5; }): "
        << endl;
    vector<int> v6 = std::views::iota(1, 9) | std::ranges::to<vector<int>>();
    auto pr1 = std::ranges::partition_point(v6, [](int& i) { return i < 5; });
    cout << "sorted 'v': " << stringify(v6) << endl;
    cout << "valor do ponto da partição 'pr1': " << *pr1 << endl;
    cout << "posição do ponto da partição 'pr1': "
         << std::distance(std::begin(v6), pr1) << endl;

    cout << endl;
    std::cout << "std::ranges::binary_search(v, 7): " << endl;
    vector<int> v7 = std::views::iota(1, 9) | std::ranges::to<vector<int>>();
    cout << "sorted 'v': " << stringify(v7) << endl;
    bool exists1 = std::ranges::binary_search(v7, 7);
    cout << "valor '7' existe? " << std::boolalpha << exists1 << endl;
    bool exists2 = std::ranges::binary_search(v7, 0);
    cout << "valor '0' existe? " << std::boolalpha << exists2 << endl;
};
}  // namespace divide_and_conquer
