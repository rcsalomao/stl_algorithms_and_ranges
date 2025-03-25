#include <algorithm>
#include <execution>
#include <iostream>
#include <print>
#include <random>
#include <ranges>
#include <vector>

namespace swaps {
using std::cout;
using std::endl;
using std::vector;

namespace Library {
struct Storage {
    int value;
};

void swap(Storage& left, Storage& right) {
    std::ranges::swap(left.value, right.value);
}
}  // namespace Library

void main() {
    cout << endl;
    int a{3};
    int b{42};
    std::ranges::swap(a, b);
    cout << "std::ranges::swap(a, b):" << endl;
    cout << "a: " << a << endl;
    cout << "b: " << b << endl;

    cout << endl;
    Library::Storage j{2};
    Library::Storage k{24};
    std::ranges::swap(j, k);
    cout << "std::ranges::swap(j, k):" << endl;
    cout << "j: " << j.value << endl;
    cout << "k: " << k.value << endl;

    cout << endl;
    auto p1 = std::make_unique<int>(7);
    auto p2 = std::make_unique<int>(9);
    int* p1_pre = p1.get();
    int* p2_pre = p2.get();
    std::ranges::iter_swap(p1,
                           p2);  // realiza uma troca indireta, do valor
                                 // referenciado pelo ponteiro.
    cout << "std::ranges::iter_swap(p1, p2):" << endl;
    cout << "*p1: " << *p1 << ", *p1_pre: " << *p1_pre << endl;
    cout << "*p2: " << *p2 << ", *p2_pre: " << *p2_pre << endl;

    cout << endl;
    vector<int> v1{1, 2, 3, 4, 56, 7};
    std::swap_ranges(std::begin(v1), std::begin(v1) + 3, std::rbegin(v1));
    cout << "std::swap_ranges(std::begin(v1), std::begin(v1) + 3, "
            "std::rbegin(v1)):"
         << endl;
    cout << "{ ";
    for (auto& e : v1) {
        cout << e << " ";
    }
    cout << "}" << endl;

    cout << endl;
    vector<int> v2 = std::views::iota(1, 9) | std::ranges::to<vector<int>>();
    vector<int> v3 = std::views::iota(11, 19) | std::ranges::to<vector<int>>();
    std::ranges::swap(v2, v3);
    cout << "std::ranges::swap(v2, v3):" << endl;
    cout << "v2: { ";
    for (auto& e : v2) {
        cout << e << " ";
    }
    cout << "}" << endl;
    cout << "v3: { ";
    for (auto& e : v3) {
        cout << e << " ";
    }
    cout << "}" << endl;
};
}  // namespace swaps
