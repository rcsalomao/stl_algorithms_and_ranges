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

namespace general_reductions {
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

struct Duck {
    string sound = "Quack";
    Duck operator+(const Duck& d) const { return {sound + d.sound}; }
};

std::string to_string(const Duck& d) { return "{" + d.sound + "}"; }

void main() {
    cout << endl;
    cout << "std::reduce(v.begin(), v.end(), 3):" << endl;
    auto v1 = vw::iota(1, 5) | rg::to<vector<int>>();
    int res1 = std::reduce(v1.begin(), v1.end(), 3);
    cout << "'v': " << stringify(v1) << endl;
    cout << "'res': " << res1 << endl;

    cout << endl;
    cout << "std::reduce(std::execution::par_unseq, v.begin(), v.end(), 3):"
         << endl;
    auto v2 = vw::iota(1, 5) | rg::to<vector<int>>();
    cout << "'v': " << stringify(v2) << endl;
    cout << "'res': "
         << std::reduce(std::execution::par_unseq, v2.begin(), v2.end(), 3)
         << endl;

    cout << endl;
    cout << "std::reduce(std::execution::par_unseq, v.begin(), v.end(), 1, "
            "std::multiplies<>{}):"
         << endl;
    auto v3 = vw::iota(1, 5) | rg::to<vector<int>>();
    cout << "'v': " << stringify(v3) << endl;
    cout << "'res': "
         << std::reduce(std::execution::par_unseq, v3.begin(), v3.end(), 1,
                        std::multiplies<>{})
         << endl;

    cout << endl;
    cout << "std::reduce(std::execution::par_unseq, v.begin(), v.end()):"
         << endl;
    vector<Duck> v4(2, Duck{});
    cout << "'v': " << stringify(v4) << endl;
    Duck res4 = std::reduce(std::execution::par_unseq, v4.begin(), v4.end());
    cout << "Duck res; 'res': " << "{" + res4.sound + "}" << endl;

    cout << endl;
    cout << "std::transform_reduce(v.begin(), v.end(), 0, std::plus<>{}, "
            "[](int i) {return i*i;}):"
         << endl;
    auto v5 = vw::iota(1, 5) | rg::to<vector<int>>();
    cout << "'v': " << stringify(v5) << endl;
    cout << "'res': "
         << std::transform_reduce(v5.begin(), v5.end(), 0, std::plus<>{},
                                  [](int i) { return i * i; })
         << endl;

    cout << endl;
    cout << "std::transform_reduce(std::execution::par_unseq, v.begin(), "
            "v.end(), w.begin(), 0, std::plus<>{}, [](int i, int j) {return "
            "i*j;}):"
         << endl;
    auto v6 = vw::iota(1, 5) | vw::transform([](auto i) { return i * 2; }) |
              rg::to<vector<int>>();
    auto w6 = vw::iota(1, 5) | vw::transform([](auto i) { return i % 2; }) |
              rg::to<vector<int>>();
    cout << "'v': " << stringify(v6) << endl;
    cout << "'w': " << stringify(w6) << endl;
    cout << "'res': "
         << std::transform_reduce(std::execution::par_unseq, v6.begin(),
                                  v6.end(), w6.begin(), 0, std::plus<>{},
                                  [](int i, int j) { return i * j; })
         << endl;

    cout << endl;
    cout << "std::inclusive_scan(v.begin(), v.end(), std::back_inserter(o), "
            "std::plus<>{}):"
         << endl;
    auto v7 = vw::iota(1, 5) | rg::to<vector<int>>();
    vector<int> o7;
    cout << "'v': " << stringify(v7) << endl;
    std::inclusive_scan(v7.begin(), v7.end(), std::back_inserter(o7),
                        std::plus<>{});
    cout << "'o': " << stringify(o7) << endl;

    cout << endl;
    cout << "std::inclusive_scan(v.begin(), v.end(), std::back_inserter(o), "
            "std::multiplies<>{}, 1):"
         << endl;
    auto v8 = vw::iota(1, 5) | rg::to<vector<int>>();
    vector<int> o8;
    cout << "'v': " << stringify(v8) << endl;
    std::inclusive_scan(v8.begin(), v8.end(), std::back_inserter(o8),
                        std::multiplies<>{}, 1);
    cout << "'o': " << stringify(o8) << endl;

    cout << endl;
    cout << "std::exclusive_scan(v.begin(), v.end(), std::back_inserter(o), 0):"
         << endl;
    auto v9 = vw::iota(1, 5) | rg::to<vector<int>>();
    vector<int> o9;
    cout << "'v': " << stringify(v9) << endl;
    std::exclusive_scan(v9.begin(), v9.end(), std::back_inserter(o9), 0);
    cout << "'o': " << stringify(o9) << endl;

    cout << endl;
    cout << "std::exclusive_scan(v.begin(), v.end(), std::back_inserter(o), 1, "
            "std::multiplies<>{}):"
         << endl;
    auto v10 = vw::iota(1, 5) | rg::to<vector<int>>();
    vector<int> o10;
    cout << "'v': " << stringify(v10) << endl;
    std::exclusive_scan(v10.begin(), v10.end(), std::back_inserter(o10), 1,
                        std::multiplies<>{});
    cout << "'o': " << stringify(o10) << endl;

    cout << endl;
    cout << "std::transform_inclusive_scan(v.begin(), v.end(), "
            "std::back_inserter(o), std::plus<>{}, [](int v){ return v * v; }, "
            "4):"
         << endl;
    auto v11 = vw::iota(1, 5) | rg::to<vector<int>>();
    vector<int> o11;
    cout << "'v': " << stringify(v11) << endl;
    std::transform_inclusive_scan(
        v11.begin(), v11.end(), std::back_inserter(o11), std::plus<>{},
        [](int v) { return v * v; }, 4);
    cout << "'o': " << stringify(o11) << endl;

    cout << endl;
    cout << "std::transform_exclusive_scan(v.begin(), v.end(), "
            "std::back_inserter(o), 4, std::plus<>{}, [](int v){ return v * v; "
            "}):"
         << endl;
    auto v12 = vw::iota(1, 5) | rg::to<vector<int>>();
    vector<int> o12;
    cout << "'v': " << stringify(v12) << endl;
    std::transform_exclusive_scan(v12.begin(), v12.end(),
                                  std::back_inserter(o12), 4, std::plus<>{},
                                  [](int v) { return v * v; });
    cout << "'o': " << stringify(o12) << endl;

    // ...
};
}  // namespace general_reductions
