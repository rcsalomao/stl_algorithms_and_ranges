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

namespace transformation {
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

struct EmptyOnMove {
    std::string value;
    EmptyOnMove(string value) : value(value) {}
    EmptyOnMove(EmptyOnMove&& src) : value(std::exchange(src.value, "-")) {}
    EmptyOnMove& operator=(EmptyOnMove&& src) {
        value = std::exchange(src.value, "-");
        return *this;
    }
    EmptyOnMove(const EmptyOnMove&) = default;
    EmptyOnMove& operator=(const EmptyOnMove&) = default;
};

std::string to_string(const EmptyOnMove& e) { return e.value; }

struct Foo {
    int x;
};

std::string to_string(const Foo& f) { return std::to_string(f.x); }

void main() {
    cout << endl;
    cout << "std::ranges::transform(v, v.begin(), [](int i) { return i * 3; }):"
         << endl;
    auto v1 = vw::iota(1, 9) | rg::to<vector<int>>();
    cout << "original 'v': " << stringify(v1) << endl;
    // vector<int> r1;
    // // realiza cópia dos valores transformados para um vetor de saída ('r1')
    // rg::transform(v1, std::back_inserter(r1), [](int i) { return i * 3; });
    // cout << "'r': " << stringify(r1) << endl;
    rg::transform(v1, v1.begin(),
                  [](int i) { return i * 3; });  // transform inplace
    cout << "transformed 'v': " << stringify(v1) << endl;

    cout << endl;
    cout << "std::ranges::transform(v, w, v.begin(), [](int i, int j) { return "
            "i * j; }):"
         << endl;
    auto v2 = vw::iota(1, 9) | rg::to<vector<int>>();
    auto w2 = vw::iota(1, 9) | rg::to<vector<int>>();
    cout << "original 'v': " << stringify(v2) << endl;
    cout << "'w': " << stringify(w2) << endl;
    // vector<int> r2;
    // // realiza cópia dos valores transformados para um vetor de saída ('r2')
    // rg::transform(v2, w2, std::back_inserter(r2),
    //               [](int i, int j) { return i * j; });
    // cout << "'r': " << stringify(r2) << endl;
    rg::transform(v2, w2, v2.begin(),
                  [](int i, int j) { return i * j; });  // transform inplace
    cout << "transformed 'v': " << stringify(v2) << endl;

    cout << endl;
    cout << "std::ranges::remove(v, 4):" << endl;
    auto v3 = vw::iota(1, 9) | rg::to<vector<int>>();
    cout << "original 'v': " << stringify(v3) << endl;
    auto [f3, l3] = rg::remove(v3, 4);
    v3.erase(f3, l3);
    cout << "removed values 'v': " << stringify(v3) << endl;

    cout << endl;
    cout << "std::ranges::remove_if(v, [](int i){return i % 2 == 0;}):" << endl;
    auto v4 = vw::iota(1, 9) | rg::to<vector<int>>();
    cout << "original 'v': " << stringify(v4) << endl;
    auto [f4, l4] = rg::remove_if(v4, [](int i) { return i % 2 == 0; });
    // v4.erase(f4, l4);
    v4.resize(std::distance(v4.begin(), f4));
    cout << "removed values 'v': " << stringify(v4) << endl;

    cout << endl;
    cout << "std::ranges::replace(v, 2, 42):" << endl;
    auto v5 = vw::iota(1, 5) | rg::to<vector<int>>();
    cout << "original 'v': " << stringify(v5) << endl;
    rg::replace(v5, 2, 42);
    cout << "replaced values 'v': " << stringify(v5) << endl;

    cout << endl;
    cout << "std::ranges::replace_if(v, [](int i){return i % 4 == 0;}, 24):"
         << endl;
    auto v6 = vw::iota(1, 9) | rg::to<vector<int>>();
    cout << "original 'v': " << stringify(v6) << endl;
    rg::replace_if(v6, [](int i) { return i % 4 == 0; }, 24);
    cout << "replaced values 'v': " << stringify(v6) << endl;

    cout << endl;
    cout << "std::ranges::reverse(v):" << endl;
    auto v7 = vw::iota(1, 9) | rg::to<vector<int>>();
    cout << "original 'v': " << stringify(v7) << endl;
    rg::reverse(v7);
    cout << "reversed 'v': " << stringify(v7) << endl;
    cout << "reverse subrange of 'v': "
         << stringify(rg::subrange(std::rbegin(v7), std::rend(v7))) << endl;

    cout << endl;
    cout << "auto v = std::span{c_array}; std::ranges::reverse(v):" << endl;
    int c_array[] = {1, 2, 3, 4, 5};
    // auto arr_view = std::span(c_array, sizeof(c_array) / sizeof(int));
    auto arr_view = std::span{c_array};
    cout << "original 'v': " << stringify(arr_view) << endl;
    rg::reverse(arr_view);
    cout << "reversed 'v': " << stringify(arr_view) << endl;
    cout << "reverse subrange of 'v': "
         << stringify(rg::subrange(std::rbegin(arr_view), std::rend(arr_view)))
         << endl;

    cout << endl;
    cout << "std::ranges::rotate(v, v.begin() + 3):" << endl;
    auto v9 = vw::iota(1, 9) | rg::to<vector<int>>();
    cout << "original 'v': " << stringify(v9) << endl;
    rg::rotate(v9, v9.begin() + 3);
    cout << "rotated 'v': " << stringify(v9) << endl;

    cout << endl;
    cout << "std::ranges::shift_right(v.begin(), v.end(), 3):" << endl;
    auto v10 = vw::iota(1, 9) | rg::to<vector<int>>();
    cout << "original 'v': " << stringify(v10) << endl;
    std::shift_right(v10.begin(), v10.end(), 3);
    cout << "shifted right 'v': " << stringify(v10) << endl;

    cout << endl;
    cout << "std::ranges::shift_left(v.begin(), v.end(), 3):" << endl;
    auto v11 = vw::iota(1, 9) | rg::to<vector<int>>();
    cout << "original 'v': " << stringify(v11) << endl;
    std::shift_left(v11.begin(), v11.end(), 3);
    cout << "shifted left 'v': " << stringify(v11) << endl;

    cout << endl;
    cout << "std::ranges::shift_right(v.begin(), v.end(), 3):" << endl;
    vector<EmptyOnMove> v12 = {
        {"a"}, {"b"}, {"c"}, {"d"}, {"e"}, {"f"}, {"g"},
    };
    cout << "original 'v': " << stringify(v12) << endl;
    std::shift_right(v12.begin(), v12.end(), 3);
    cout << "shifted right 'v': " << stringify(v12) << endl;

    cout << endl;
    cout << "std::mt19937 rng{std::random_device{}()}; std::ranges::shuffle(v, "
            "rng):"
         << endl;
    // vector<Foo> v13(10);
    // rg::generate(v13, [i = 0]() mutable { return Foo{++i}; });
    auto v13 = vw::iota(1, 11) | vw::transform([](int i) { return Foo{i}; }) |
               rg::to<vector<Foo>>();
    cout << "original 'v': " << stringify(v13) << endl;
    std::mt19937 rng{std::random_device{}()};
    rg::shuffle(v13, rng);
    cout << "shuffled 'v': " << stringify(v13) << endl;

    cout << endl;
    cout << "do{cout << v << endl;} "
            "while(std::ranges::next_permutation(v).found):"
         << endl;
    auto v14 = vw::iota(1, 4) | rg::to<vector<int>>();
    do {
        cout << "  'v' permutation: " << stringify(v14) << endl;
    } while (rg::next_permutation(v14).found);

    cout << endl;
    cout << "std::ranges::is_permutation(w, v):" << endl;
    auto v15 = vw::iota(1, 9) | rg::to<vector<int>>();
    rg::shuffle(v15, std::random_device{});
    cout << "'v': " << stringify(v15) << endl;
    auto w15 = vw::iota(1, 9) | rg::to<vector<int>>();
    rg::shuffle(w15, std::random_device{});
    cout << "'w': " << stringify(w15) << endl;
    cout << "is 'w' a permutation of 'v'? " << std::boolalpha
         << rg::is_permutation(w15, v15) << endl;
    auto u15 = vw::iota(1, 9) | vw::transform([](int i) { return i % 3; }) |
               rg::to<vector<int>>();
    cout << "'u': " << stringify(u15) << endl;
    cout << "is 'u' a permutation of 'v'? " << std::boolalpha
         << rg::is_permutation(u15, v15) << endl;
};
}  // namespace transformation
