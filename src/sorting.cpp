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

namespace sorting {
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

struct Account {
    double _value{0};
    Account(double i) : _value{i} {};
    double value() const { return _value; }
};

struct Record {
    std::string label;
    int rank;
};

void main() {
    cout << endl;
    cout << "std::sort(v.begin(), v.end()):" << endl;
    vector<int> v1 = views::iota(1, 5) | std::ranges::to<vector<int>>();
    std::ranges::shuffle(v1, std::random_device{});
    cout << "unsorted 'v': " << stringify(v1) << endl;
    std::sort(std::begin(v1), std::end(v1));
    cout << "sorted 'v': " << stringify(v1) << endl;

    cout << endl;
    cout << "std::list<int> l{...}; l.sort():" << endl;
    std::list<int> l1 = views::iota(1, 5) | std::ranges::to<std::list<int>>();
    vector<std::reference_wrapper<int>> rl1(std::begin(l1), std::end(l1));
    std::ranges::shuffle(rl1, std::random_device{});
    std::list<int> l1a(std::begin(rl1), std::end(rl1));
    cout << "unsorted 'l': " << stringify(l1a) << endl;
    l1a.sort();
    cout << "sorted 'l': " << stringify(l1a) << endl;

    cout << endl;
    cout << "std::vector<Account> v{...}; std::ranges::sort(v, "
            "std::greater<>{}, &Account::value):"
         << endl;
    std::vector<Account> v2 =
        views::iota(1, 5) |
        views::transform([](int i) { return Account{i * 2.4}; }) |
        std::ranges::to<std::vector<Account>>();
    std::ranges::shuffle(v2, std::random_device{});
    cout << "unsorted 'v': " << stringify(v2, [](const Account& a) {
        return std::to_string(a.value());
    }) << endl;
    std::ranges::sort(v2, std::greater<>{}, &Account::value);
    cout << "sorted 'v': " << stringify(v2, [](const Account& a) {
        return std::to_string(a.value());
    }) << endl;

    cout << endl;
    cout << "std::vector<Record> v{...}; std::ranges::stable_sort(v, {}, "
            "&Record::label); std::ranges::stable_sort(v, {}, &Record::rank):"
         << endl;
    vector<Record> v3 = {
        {"q", 1}, {"f", 1}, {"c", 2}, {"a", 1}, {"d", 3},
    };
    cout << "unsorted 'v': " << stringify(v3, [](const Record& r) {
        return "{" + r.label + "," + std::to_string(r.rank) + "}";
    }) << endl;
    std::ranges::stable_sort(v3, {}, &Record::label);
    cout << "sorted (&Record::label) 'v': "
         << stringify(v3,
                      [](const Record& r) {
                          return "{" + r.label + "," + std::to_string(r.rank) +
                                 "}";
                      })
         << endl;
    std::ranges::stable_sort(v3, {}, &Record::rank);
    cout << "sorted (&Record::rank) 'v': "
         << stringify(v3,
                      [](const Record& r) {
                          return "{" + r.label + "," + std::to_string(r.rank) +
                                 "}";
                      })
         << endl;

    cout << endl;
    cout << "std::is_sorted(std::begin(v), std::end(v)):" << endl;
    vector<int> v4 = views::iota(1, 5) | std::ranges::to<vector<int>>();
    cout << "'v': " << stringify(v4) << endl;
    cout << "is 'v' sorted?: " << std::boolalpha
         << std::is_sorted(std::begin(v4), std::end(v4)) << endl;

    cout << endl;
    cout << "std::is_sorted(std::begin(v), std::end(v)):" << endl;
    vector<int> v5 = views::iota(1, 5) | std::ranges::to<vector<int>>();
    std::ranges::shuffle(v5, std::random_device{});
    cout << "'v': " << stringify(v5) << endl;
    cout << "is 'v' sorted?: " << std::boolalpha
         << std::is_sorted(std::begin(v5), std::end(v5)) << endl;

    cout << endl;
    cout << "std::ranges::is_sorted(v, std::greater<>{}):" << endl;
    vector<int> v6 =
        views::iota(1, 5) | views::reverse | std::ranges::to<vector<int>>();
    cout << "'v': " << stringify(v6) << endl;
    cout << "is 'v' reverse sorted?: " << std::boolalpha
         << std::ranges::is_sorted(v6, std::greater<>{}) << endl;

    cout << endl;
    cout << "std::ranges::is_sorted_until(v):" << endl;
    vector<int> v7 = views::iota(1, 5) | std::ranges::to<vector<int>>();
    std::ranges::shuffle(v7, std::random_device{});
    cout << "'v': " << stringify(v7) << endl;
    auto it = std::ranges::is_sorted_until(v7);
    cout << "is sorted until: " << *it << endl;

    cout << endl;
    cout << "std::ranges::partial_sort(v, v.begin() + 4, std::greater<>{}):"
         << endl;
    vector<int> v8 = views::iota(1, 10) | std::ranges::to<vector<int>>();
    std::ranges::shuffle(v8, std::random_device{});
    cout << "unsorted 'v': " << stringify(v8) << endl;
    // std::ranges::partial_sort(v8, v8.begin() + 4, std::greater<>{});
    std::ranges::partial_sort(v8, std::next(std::begin(v8), 4),
                              std::greater<>{});
    cout << "partially sorted 'v': " << stringify(v8) << endl;

    cout << endl;
    cout << "std::ranges::partial_sort_copy(v, w, std::greater<>{}):" << endl;
    vector<int> v9 = views::iota(1, 10) | std::ranges::to<vector<int>>();
    vector<int> v10(4);
    std::ranges::shuffle(v9, std::random_device{});
    cout << "unsorted 'v': " << stringify(v9) << endl;
    // std::ranges::partial_sort_copy(v9.begin(), v9.end(), v10.begin(),
    // v10.end(), std::greater<>{});
    std::ranges::partial_sort_copy(v9, v10, std::greater<>{});
    cout << "partially sorted copy on 'w': " << stringify(v10) << endl;

    cout << endl;
    cout << "std::partial_sort_copy(v.begin(), v.end(), w.begin(), w.end(), "
            "std::greater<>{}):"
         << endl;
    vector<int> v11 = views::iota(20, 24) | std::ranges::to<vector<int>>();
    vector<int> v12 = views::iota(1, 10) | std::ranges::to<vector<int>>();
    std::ranges::shuffle(v11, std::random_device{});
    cout << "unsorted 'v': " << stringify(v11) << endl;
    cout << "unsorted 'w': " << stringify(v12) << endl;
    auto it2 = std::partial_sort_copy(v11.begin(), v11.end(), v12.begin(),
                                      v12.end(), std::greater<>{});
    cout << "partially sorted copy on 'w': " << stringify(v12) << endl;
    cout << "value of the resulting iterator: " << *it2 << endl;
};
}  // namespace sorting
