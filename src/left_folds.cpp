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

namespace left_folds {
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
    cout << "std::accumulate(v.begin(), v.end(), 0):" << endl;
    auto v1 = vw::iota(1, 6) | rg::to<vector<int>>();
    auto sum1 = std::accumulate(v1.begin(), v1.end(), 0);
    cout << "'v': " << stringify(v1) << endl;
    cout << "sum accum. of 'v': " << sum1 << endl;

    cout << endl;
    cout << "std::accumulate(v.rbegin(), v.rend(), 0, [](int acc, int "
            "el){return acc/2 + el;}):"
         << endl;
    auto v2 = vw::iota(1, 6) | rg::to<vector<int>>();
    auto sum2 = std::accumulate(v2.rbegin(), v2.rend(), 0,
                                [](int acc, int el) { return acc / 2 + el; });
    cout << "'v': " << stringify(v2) << endl;
    cout << "right fold sum accum. of 'v': " << sum2 << endl;

    cout << endl;
    cout << "std::inner_product(v.begin(), v.end(), w.begin(), 0)" << endl;
    auto v3 = vw::iota(1, 6) | rg::to<vector<int>>();
    auto w3 = vw::iota(2, 7) | rg::to<vector<int>>();
    auto sum3 = std::inner_product(v3.begin(), v3.end(), w3.begin(), 0);
    cout << "'v': " << stringify(v3) << endl;
    cout << "'w': " << stringify(w3) << endl;
    cout << "inner product: " << sum3 << endl;

    cout << endl;
    cout << "std::inner_product( v.begin(), std::prev(std::end(v)), "
            "std::next(std::begin(v)), 0, std::plus<>{}, [](int left, int "
            "right) { return std ::abs(left - right); })"
         << endl;
    vector<int> v4{6, 4, 3, 7, 2, 1};
    auto sum4 = std::inner_product(
        v4.begin(), std::prev(std::end(v4)), std::next(std::begin(v4)), 0,
        std::plus<>{},
        [](int left, int right) { return std ::abs(left - right); });
    cout << "'v': " << stringify(v4) << endl;
    cout << "sum of differences: " << sum4 << endl;

    cout << endl;
    cout << "" << endl;
    vector<int> v5(6);
    rg::generate(v5, []() { return std::random_device{}() / 100000000; });
    cout << "original 'v': " << stringify(v5) << endl;
    std::partial_sum(v5.begin(), v5.end(), v5.begin());
    cout << "partially summed 'v': " << stringify(v5) << endl;

    cout << endl;
    cout << "" << endl;
    vector<int> v6(6);
    rg::generate(v6, []() { return std::random_device{}() / 100000000; });
    // v6[0] = std::ranges::max({1, v6[0]});
    v6[0] = std::ranges::max(1, v6[0]);
    vector<int> w6;
    cout << "'v': " << stringify(v6) << endl;
    std::partial_sum(v6.begin(), v6.end(), std::back_inserter(w6),
                     std::multiplies<>{});
    cout << "'w': " << stringify(w6) << endl;

    cout << endl;
    cout << "std::adjacent_difference(v.begin(), v.end(), v.begin()):" << endl;
    vector<int> v7(6);
    rg::generate(v7, []() { return std::random_device{}() / 100000000; });
    cout << "original 'v': " << stringify(v7) << endl;
    // vector<int> w7;
    // std::adjacent_difference(v7.begin(), v7.end(), std::back_inserter(w7));
    // cout << "adjacent difference of 'v': " << stringify(w7) << endl;
    std::adjacent_difference(v7.begin(), v7.end(), v7.begin());
    cout << "adjacent difference of 'v': " << stringify(v7) << endl;

    cout << endl;
    cout << "std::adjacent_difference(v.begin(), std::prev(v.end()), "
            "std::next(v.begin()), std::plus<>{}):"
         << endl;
    vector<int> v8(10, 1);
    cout << "original 'v': " << stringify(v8) << endl;
    std::adjacent_difference(v8.begin(), std::prev(v8.end()),
                             std::next(v8.begin()), std::plus<>{});
    cout << "fibonnacci sequence from 'v': " << stringify(v8) << endl;

    cout << endl;
    cout << "std::adjacent_difference(std::execution::par_unseq, v.begin(), "
            "v.end(), w.begin()):"
         << endl;
    auto v9 =
        vw::iota(0, 6) |
        vw::transform([](int) { return std::random_device{}() / 100000000; }) |
        rg::to<vector<int>>();
    cout << "original 'v': " << stringify(v9) << endl;
    vector<int> w9(v9.size());
    std::adjacent_difference(std::execution::par_unseq, v9.begin(), v9.end(),
                             w9.begin());
    cout << "adjacent difference of 'v': " << stringify(w9) << endl;
};
}  // namespace left_folds
