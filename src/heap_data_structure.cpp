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

namespace heap_data_structure {
using boost::typeindex::type_id_with_cvr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
namespace rg = std::ranges;
namespace vw = std::views;

// ADL
using std::to_string;
std::string_view to_string(std::string_view s) { return s; };

template <std::ranges::forward_range Rng>
auto stringify(Rng&& seq) {
    auto b = seq | vw::transform([](const auto& a) { return to_string(a); }) |
             vw::join_with(',') | vw::common;
    return "{" + std::string(std::begin(b), std::end(b)) + "}";
}

template <typename Cmp = rg::greater>
auto top_k_heap(std::input_iterator auto begin,
                std::sentinel_for<decltype(begin)> auto end, size_t k,
                Cmp cmp = {}) {
    vector<std::iter_value_t<decltype(begin)>> result;
    while (begin != end) {
        result.push_back(*begin);
        rg::push_heap(result, cmp);
        if (result.size() > k) {
            rg::pop_heap(result, cmp);
            result.pop_back();
        }
        std::advance(begin, 1);
    }
    rg::sort_heap(result, cmp);
    return result;
}

void main() {
    {
        cout << endl;
        auto v = vw::iota(0, 6) | rg::to<vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "original 'v': " << stringify(v) << endl;
        std::ranges::make_heap(v);
        cout << "std::ranges::make_heap(v): " << stringify(v) << endl;
        std::ranges::make_heap(v, std::greater<>{});
        cout << "std::ranges::make_heap(v, std::greater<>{}): " << stringify(v)
             << endl;
    };
    {
        cout << endl;
        auto v = vw::iota(0, 6) | rg::to<vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "original 'v': " << stringify(v) << endl;
        std::ranges::make_heap(v);
        cout << "std::ranges::make_heap(v): " << stringify(v) << endl;
        v.push_back(42);
        cout << "v.push_back(42): " << stringify(v) << endl;
        std::ranges::push_heap(v);
        cout << "std::ranges::push_heap(v): " << stringify(v) << endl;
        v.push_back(24);
        cout << "v.push_back(24): " << stringify(v) << endl;
        std::ranges::push_heap(v);
        cout << "std::ranges::push_heap(v): " << stringify(v) << endl;
    };
    {
        cout << endl;
        auto v = vw::iota(0, 6) | rg::to<vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "original 'v': " << stringify(v) << endl;
        std::ranges::make_heap(v);
        cout << "std::ranges::make_heap(v): " << stringify(v) << endl;
        v.push_back(42);
        cout << "v.push_back(42): " << stringify(v) << endl;
        std::ranges::push_heap(v);
        cout << "std::ranges::push_heap(v): " << stringify(v) << endl;
        std::ranges::pop_heap(v);
        cout << "std::ranges::pop_heap(v): " << stringify(v) << endl;
        v.pop_back();
        cout << "v.pop_back(): " << stringify(v) << endl;
    };
    {
        cout << endl;
        auto v = vw::iota(0, 6) | rg::to<vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "original 'v': " << stringify(v) << endl;
        std::ranges::make_heap(v);
        cout << "std::ranges::make_heap(v): " << stringify(v) << endl;
        std::ranges::sort_heap(v);
        cout << "std::ranges::sort_heap(v): " << stringify(v) << endl;
    };
    {
        cout << endl;
        auto v = vw::iota(0, 6) | rg::to<vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "original 'v': " << stringify(v) << endl;
        cout << "std::ranges::is_heap(v): " << std::boolalpha
             << std::ranges::is_heap(v) << endl;
        std::ranges::make_heap(v);
        cout << "std::ranges::make_heap(v): " << stringify(v) << endl;
        cout << "std::ranges::is_heap(v): " << std::boolalpha
             << std::ranges::is_heap(v) << endl;
    };
    {
        cout << endl;
        auto v = vw::iota(0, 6) | rg::to<vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "original 'v': " << stringify(v) << endl;
        std::ranges::make_heap(v);
        cout << "std::ranges::make_heap(v): " << stringify(v) << endl;
        for (auto& a : {24, 42, 19}) {
            v.push_back(a);
        }
        cout << "v.push_back({24, 42, 19}): " << stringify(v) << endl;
        auto it = std::ranges::is_heap_until(v);
        cout << "std::ranges::is_heap_until(v): " << *it << endl;
    };
    //  Comparando-se com a estrutura de dados 'std::priority_queue', pode-se
    //  perceber que 'std::priority_queue' provém uma interface amigável para as
    //  operações mais comuns com 'heaps', enquanto que os algoritimos
    //  'std::make_heap' e etc permitem acesso imediato à estrutura de dados
    //  subjacente, ao custo de gerenciamento manual da 'heap'.
    {
        cout << endl;
        auto v = vw::iota(0, 6) | rg::to<vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "original 'v': " << stringify(v) << endl;
        cout << "top 3 values: " << stringify(top_k_heap(v.begin(), v.end(), 3))
             << endl;
        cout << "lesser 3 values: "
             << stringify(top_k_heap(v.begin(), v.end(), 3, std::less<>{}))
             << endl;
    }
};
}  // namespace heap_data_structure
