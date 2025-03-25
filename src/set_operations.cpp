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

namespace set_operations {
using boost::typeindex::type_id_with_cvr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using namespace std::ranges;

using std::to_string;  // ADL

template <std::ranges::forward_range Rng>
auto stringify(Rng&& seq) {
    auto b = seq |
             views::transform([](const auto& a) { return to_string(a); }) |
             views::join_with(',') | views::common;
    return "{" + std::string(std::begin(b), std::end(b)) + "}";
}

struct LabeledValue {
    string label{};
    int value{};
};

std::string to_string(const LabeledValue& l) {
    return "{" + l.label + ", " + std::to_string(l.value) + "}";
};

void main() {
    cout << endl;
    cout << "std::set_difference(v, w, std::back_inserter(r)):" << endl;
    // as ranges de entradas precisam estar ordenadas.
    auto v1a = views::iota(0, 6) | std::ranges::to<vector<int>>();
    auto v1b = views::iota(2, 4) | std::ranges::to<vector<int>>();
    vector<int> diff1;
    cout << "'v': " << stringify(v1a) << endl;
    cout << "'w': " << stringify(v1b) << endl;
    std::ranges::set_difference(v1a, v1b, std::back_inserter(diff1));
    cout << "'r': " << stringify(diff1) << endl;

    cout << endl;
    cout << "std::set_difference(v, w, std::back_inserter(r), cmp):" << endl;
    vector<LabeledValue> v2 = {
        {"primeiro_a", 1},
        {"primeiro_b", 1},
        {"primeiro_c", 1},
        {"primeiro_d", 1},
    };
    vector<LabeledValue> w2 = {
        {"segundo_a", 1},
        {"segundo_b", 1},
    };
    vector<LabeledValue> r2;
    auto cmp2 = [](const LabeledValue& l, const LabeledValue& r) {
        return l.value < r.value;
    };
    cout << "'v': " << stringify(v2) << endl;
    cout << "'w': " << stringify(w2) << endl;
    // std::ranges::set_difference(v2, w2, std::back_inserter(r2), {},
    //                             &LabeledValue::value, &LabeledValue::value);
    std::ranges::set_difference(v2, w2, std::back_inserter(r2), cmp2);
    cout << "'r': " << stringify(r2) << endl;

    cout << endl;
    cout << "std::set_symmetric_difference(v, w, std::back_inserter(r)):"
         << endl;
    // as ranges de entradas precisam estar ordenadas.
    vector<int> v3 = {1, 3, 5, 7, 9};
    vector<int> w3 = {3, 4, 5, 6, 7};
    vector<int> r3;
    cout << "'v': " << stringify(v3) << endl;
    cout << "'w': " << stringify(w3) << endl;
    std::ranges::set_symmetric_difference(v3, w3, std::back_inserter(r3));
    cout << "'r': " << stringify(r3) << endl;

    cout << endl;
    cout << "std::set_symmetric_difference(v, w, std::back_inserter(r), cmp):"
         << endl;
    vector<LabeledValue> v4 = {
        {"primeiro_a", 1},
        {"primeiro_b", 2},
        {"primeiro_c", 2},
    };
    vector<LabeledValue> w4 = {
        {"segundo_a", 1},
        {"segundo_b", 1},
        {"segundo_c", 2},
    };
    vector<LabeledValue> r4;
    auto cmp4 = [](const LabeledValue& l, const LabeledValue& r) {
        return l.value < r.value;
    };
    cout << "'v': " << stringify(v4) << endl;
    cout << "'w': " << stringify(w4) << endl;
    // std::ranges::set_symmetric_difference(v4, w4, std::back_inserter(r4), {},
    //                                       &LabeledValue::value,
    //                                       &LabeledValue::value);
    std::ranges::set_symmetric_difference(v4, w4, std::back_inserter(r4), cmp4);
    cout << "'r': " << stringify(r4) << endl;

    cout << endl;
    cout << "std::set_union(v, w, std::back_inserter(r)):" << endl;
    // as ranges de entradas precisam estar ordenadas.
    vector<int> v5 = {1, 3, 5};
    vector<int> w5 = {2, 4, 6};
    vector<int> r5;
    cout << "'v': " << stringify(v5) << endl;
    cout << "'w': " << stringify(w5) << endl;
    std::ranges::set_union(v5, w5, std::back_inserter(r5));
    cout << "'r': " << stringify(r5) << endl;

    cout << endl;
    cout << "std::set_union(v, w, std::back_inserter(r), cmp):" << endl;
    vector<LabeledValue> v6 = {
        {"primeiro_a", 1},
        {"primeiro_b", 1},
        {"primeiro_c", 2},
    };
    vector<LabeledValue> w6 = {
        {"segundo_a", 1},
        {"segundo_b", 2},
        {"segundo_c", 2},
    };
    vector<LabeledValue> r6;
    auto cmp6 = [](const LabeledValue& l, const LabeledValue& r) {
        return l.value < r.value;
    };
    cout << "'v': " << stringify(v6) << endl;
    cout << "'w': " << stringify(w6) << endl;
    // std::ranges::set_union(v6, w6, std::back_inserter(r6), {},
    //                                       &LabeledValue::value,
    //                                       &LabeledValue::value);
    std::ranges::set_union(v6, w6, std::back_inserter(r6), cmp6);
    cout << "'r': " << stringify(r6) << endl;

    cout << endl;
    cout << "std::set_intersection(v, w, std::back_inserter(r)):" << endl;
    // as ranges de entradas precisam estar ordenadas.
    vector<int> v7 = {1, 2, 3, 4, 5};
    vector<int> w7 = {2, 4, 6};
    vector<int> r7;
    cout << "'v': " << stringify(v7) << endl;
    cout << "'w': " << stringify(w7) << endl;
    std::ranges::set_intersection(v7, w7, std::back_inserter(r7));
    cout << "'r': " << stringify(r7) << endl;

    cout << endl;
    cout << "std::set_intersection(v, w, std::back_inserter(r), cmp):" << endl;
    vector<LabeledValue> v8 = {
        {"primeiro_a", 1},
        {"primeiro_b", 2},
    };
    vector<LabeledValue> w8 = {
        {"segundo_a", 1},
        {"segundo_b", 2},
        {"segundo_c", 2},
    };
    vector<LabeledValue> r8;
    auto cmp8 = [](const LabeledValue& l, const LabeledValue& r) {
        return l.value < r.value;
    };
    cout << "'v': " << stringify(v8) << endl;
    cout << "'w': " << stringify(w8) << endl;
    // std::ranges::set_intersection(v8, w8, std::back_inserter(r8), {},
    //                                       &LabeledValue::value,
    //                                       &LabeledValue::value);
    std::ranges::set_intersection(v8, w8, std::back_inserter(r8), cmp8);
    cout << "'r': " << stringify(r8) << endl;
};
}  // namespace set_operations
