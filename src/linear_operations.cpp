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

namespace linear_operations {
using boost::typeindex::type_id_with_cvr;
using std::cout;
using std::endl;
using std::string;
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

struct LabeledValue {
    int value{};
    string label{};
};

void main() {
    cout << endl;
    cout << "std::ranges::includes(v, w):" << endl;
    auto v1 = views::iota(1, 9) | std::ranges::to<std::vector<int>>();
    auto w1 = views::iota(1, 9) |
              views::filter([](auto a) { return a % 3 == 0; }) |
              std::ranges::to<std::vector<int>>();
    vector<int> u1{1, 2, 2, 4, 24};
    // ambas sequências precisam estar ordenadas.
    cout << "sorted 'v': " << stringify(v1) << endl;
    cout << "sorted 'w': " << stringify(w1) << endl;
    cout << "sorted 'u': " << stringify(u1) << endl;
    cout << "std::ranges::includes(v, w): " << std::boolalpha
         << std::ranges::includes(v1, w1) << endl;
    cout << "std::ranges::includes(v, u): " << std::boolalpha
         << std::ranges::includes(v1, u1) << endl;

    cout << endl;
    cout << "std::ranges::merge(va, vb, std::back_inserter(r), {}, "
            "&LabeledValue::value, &LabeledValue::value):"
         << endl;
    vector<LabeledValue> v2a{
        {1, "primeiro"},
        {2, "primeiro"},
        {3, "primeiro"},
    };
    vector<LabeledValue> v2b{
        {0, "segundo"},
        {2, "segundo"},
        {4, "segundo"},
    };
    vector<LabeledValue> r2;
    // auto cmp2 = [](const LabeledValue& l, const LabeledValue& r) {
    //     return l.value < r.value;
    // };
    // std::ranges::merge(v2a, v2b, std::back_inserter(r2), cmp2);
    std::ranges::merge(v2a, v2b, std::back_inserter(r2), {},
                       &LabeledValue::value, &LabeledValue::value);
    cout << "'va': " << stringify(v2a, [](const LabeledValue& l) {
        return "{" + std::to_string(l.value) + ", " + l.label + "}";
    }) << endl;
    cout << "'vb': " << stringify(v2b, [](const LabeledValue& l) {
        return "{" + std::to_string(l.value) + ", " + l.label + "}";
    }) << endl;
    cout << "'r': " << stringify(r2, [](const LabeledValue& l) {
        return "{" + std::to_string(l.value) + ", " + l.label + "}";
    }) << endl;

    cout << endl;
    cout << "std::merge(std::execution::par_unseq, va.begin(), va.end(), "
            "vb.begin(), vb.end(), r.begin(), cmp):"
         << endl;
    vector<LabeledValue> v3a{
        {1, "primeiro"},
        {2, "primeiro"},
        {3, "primeiro"},
    };
    vector<LabeledValue> v3b{
        {0, "segundo"},
        {2, "segundo"},
        {4, "segundo"},
    };
    vector<LabeledValue> r3(v3a.size() + v3b.size());
    auto cmp3 = [](const LabeledValue& l, const LabeledValue& r) {
        return l.value < r.value;
    };
    std::merge(std::execution::par_unseq, v3a.begin(), v3a.end(), v3b.begin(),
               v3b.end(), r3.begin(), cmp3);
    cout << "'va': " << stringify(v3a, [](const LabeledValue& l) {
        return "{" + std::to_string(l.value) + ", " + l.label + "}";
    }) << endl;
    cout << "'vb': " << stringify(v3b, [](const LabeledValue& l) {
        return "{" + std::to_string(l.value) + ", " + l.label + "}";
    }) << endl;
    cout << "'r': " << stringify(r3, [](const LabeledValue& l) {
        return "{" + std::to_string(l.value) + ", " + l.label + "}";
    }) << endl;

    cout << endl;
    cout << "std::inplace_merge(v.begin(), v.begin() + 3, v.end()):" << endl;
    vector<int> v4{1, 3, 5, 2, 4, 6};
    cout << "'v': " << stringify(v4) << endl;
    std::inplace_merge(v4.begin(), v4.begin() + 3, v4.end());
    cout << "'v' after 'std::inplace_merge()': " << stringify(v4) << endl;

    cout << endl;
    cout << "std::ranges::unique(v):" << endl;
    vector<int> v5{1, 1, 2, 2, 3, 4, 5, 6, 6, 6};
    std::ranges::shuffle(v5, std::random_device{});
    std::ranges::sort(v5);
    cout << "'v': " << stringify(v5) << endl;
    // auto it = std::unique(v5.begin(), v5.end());
    // v5.resize(std::distance(v5.begin(), it));
    auto [first, last] = std::ranges::unique(v5);
    v5.erase(first, last);
    cout << "unique 'v' elements: " << stringify(v5) << endl;

    cout << endl;
    cout << "std::ranges::unique_copy(v, std::back_inserter(out)):" << endl;
    vector<int> v6{1, 1, 2, 2, 3, 4, 5, 6, 6, 6};
    std::ranges::shuffle(v6, std::random_device{});
    std::ranges::sort(v6);
    vector<int> out6;
    cout << "'v': " << stringify(v6) << endl;
    std::ranges::unique_copy(v6, std::back_inserter(out6));
    cout << "'out': " << stringify(out6) << endl;
};
}  // namespace linear_operations
