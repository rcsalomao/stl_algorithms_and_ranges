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

namespace boolean_reductions {
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
    cout << "std::ranges::all_of(v, [](int v){return v % 2 == 0;}): " << endl;
    auto v1 = vw::iota(0, 5) | vw::transform([](int i) { return i * 2; }) |
              rg::to<vector<int>>();
    cout << "'v': " << stringify(v1) << endl;
    cout << "'res': " << std::boolalpha
         << rg::all_of(v1, [](int v) { return v % 2 == 0; }) << endl;

    cout << endl;
    cout << "std::ranges::any_of(v, [](int v){return std::signbit(v);}): "
         << endl;
    auto v2 = vw::iota(0, 5) | vw::transform([](int i) { return i * 2; }) |
              rg::to<vector<int>>();
    cout << "'v': " << stringify(v2) << endl;
    cout << "'res': " << std::boolalpha
         << rg::any_of(v2, [](int v) { return std::signbit(v); }) << endl;

    cout << endl;
    cout << "std::ranges::none_of(v, [](int v){return v > 8;}): " << endl;
    auto v3 = vw::iota(0, 5) | vw::transform([](int i) { return i * 2; }) |
              rg::to<vector<int>>();
    cout << "'v': " << stringify(v3) << endl;
    cout << "'res': " << std::boolalpha
         << rg::none_of(v3, [](int v) { return v > 8; }) << endl;

    // ...
};
}  // namespace boolean_reductions
