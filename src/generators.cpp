#include <algorithm>
#include <boost/type_index.hpp>
#include <concepts>
#include <execution>
#include <iostream>
#include <list>
#include <numeric>
#include <print>
#include <random>
#include <ranges>
#include <typeinfo>
#include <vector>

namespace generators {
using boost::typeindex::type_id_with_cvr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
namespace rg = std::ranges;
namespace vw = std::views;

using std::to_string;  // ADL

template <std::ranges::forward_range Rng>
auto stringify(Rng&& seq) {
    auto b = seq | vw::transform([](const auto& a) { return to_string(a); }) |
             vw::join_with(',') | vw::common;
    return "{" + std::string(std::begin(b), std::end(b)) + "}";
}

void main() {
    cout << endl;
    cout << "std::ranges::fill(v, 24):" << endl;
    vector<int> v1(5);
    cout << "original 'v': " << stringify(v1) << endl;
    rg::fill(v1, 24);
    cout << "filled 'v': " << stringify(v1) << endl;
    cout << "std::ranges::fill(rg::subrange(v.begin() + 2, v.begin() + 4), "
            "42):"
         << endl;
    rg::fill(rg::subrange(v1.begin() + 2, v1.begin() + 4), 42);
    cout << "refilled 'v': " << stringify(v1) << endl;

    cout << endl;
    cout << "std::ranges::generate(v, [i=0]() mutable {return (++i) % 2;}):"
         << endl;
    vector<int> v2(5);
    cout << "original 'v': " << stringify(v2) << endl;
    rg::generate(v2, [i = 0]() mutable { return (++i) % 2; });
    cout << "generated 'v': " << stringify(v2) << endl;

    cout << endl;
    cout << "std::ranges::fill_n(std::back_inserter(v), 5, 42):" << endl;
    vector<int> v3;
    cout << "original 'v': " << stringify(v3) << endl;
    rg::fill_n(std::back_inserter(v3), 5, 42);
    cout << "filled 'v': " << stringify(v3) << endl;
    cout << "std::ranges::fill_n(std::next(std::begin(v), 1), 2, 24):" << endl;
    rg::fill_n(std::next(std::begin(v3), 1), 2, 24);
    cout << "refilled 'v': " << stringify(v3) << endl;

    cout << endl;
    cout << "std::ranges::generate_n(std::back_inserter(v), 5, [i = 0]() "
            "mutable { return 42 * ((++i) % 2); }):"
         << endl;
    vector<int> v4;
    cout << "original 'v': " << stringify(v4) << endl;
    rg::generate_n(std::back_inserter(v4), 5,
                   [i = 0]() mutable { return 42 * ((++i) % 2); });
    cout << "generated 'v': " << stringify(v4) << endl;

    cout << endl;
    cout << "std::iota(v.begin(), v.end(), 5):" << endl;
    vector<int> v5(5);
    cout << "original 'v': " << stringify(v5) << endl;
    std::iota(v5.begin(), v5.end(), 5);
    cout << "modified 'v': " << stringify(v5) << endl;

    cout << endl;
    cout << "rg::transform(vw::iota(1, 10), vw::iota(5), "
            "std::back_inserter(v), std::plus<>{}):"
         << endl;
    vector<int> v6;
    cout << "original 'v': " << stringify(v6) << endl;
    rg::transform(vw::iota(1, 10), vw::iota(5), std::back_inserter(v6),
                  std::plus<>{});
    cout << "transformed 'v': " << stringify(v6) << endl;
};
}  // namespace generators
