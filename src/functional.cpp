#include <algorithm>
#include <execution>
#include <iostream>
#include <print>
#include <random>
#include <ranges>
#include <vector>

struct Functor {
    int cnt{0};
    int sum{0};
    void operator()(int v) {
        cnt++;
        sum += v;
    }
};

namespace functional {
using std::cout;
using std::endl;
using std::vector;

void main() {
    cout << endl;
    cout << "std::for_each(v.begin(), v.end(), invocable):" << endl;
    vector<int> v1{1, 2, 3, 4, 5};
    auto result1 = std::for_each(v1.begin(), v1.end(), Functor{});
    cout << "{result.cnt; result.sum} = {" << result1.cnt << "; " << result1.sum
         << "}" << endl;

    cout << endl;
    cout << "std::for_each(v.begin(), v.end(), [&](int e){...}):" << endl;
    vector<int> v2{1, 2, 3, 4, 5, 6};
    int cnt{0};
    int sum{0};
    std::for_each(v2.begin(), v2.end(), [&cnt, &sum](int e) {
        cnt++;
        sum += e;
    });
    cout << "{cnt; sum} = {" << cnt << "; " << sum << "}" << endl;

    cout << endl;
    cout << "std::for_each(std::execution::par_unseq, v.begin(), v.end(), "
            "[&](int e){...}):"
         << endl;
    vector<int> v3{1, 2, 3, 4, 5, 6, 7};
    std::atomic<int> acnt{0};
    std::atomic<int> asum{0};
    std::for_each(std::execution::par_unseq, v3.begin(), v3.end(),
                  [&acnt, &asum](int e) {
                      acnt++;
                      asum += e;
                  });
    cout << "{acnt; asum} = {" << acnt << "; " << asum << "}" << endl;

    cout << endl;
    cout << "std::ranges::for_each(v, [&](int e){...}):" << endl;
    int rcnt{0};
    int rsum{0};
    vector<int> v4{1, 2, 3, 4, 5, 6, 7, 8};
    std::ranges::for_each(v4, [&rcnt, &rsum](int e) {
        rcnt++;
        rsum += e;
    });
    cout << "{rcnt; rsum} = {" << rcnt << "; " << rsum << "}" << endl;

    cout << endl;
    cout << "std::ranges::for_each(v, [](auto& e){e *= 3;}):" << endl;
    vector<int> v5{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::ranges::for_each(v5, [](auto& e) { e *= 3; });
    cout << "v: { ";
    for (auto& e : v5) {
        cout << e << " ";
    }
    cout << "}" << endl;

    cout << endl;
    cout << "std::ranges::for_each(v, Functor{}):" << endl;
    vector<int> v6{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto result2 = (std::ranges::for_each(v6, Functor{})).fun;
    cout << "{result.cnt; result.sum} = {" << result2.cnt << "; " << result2.sum
         << "}" << endl;

    cout << endl;
    cout << "std::for_each_n(std::execution::par_unseq, std::begin(v), "
            "v.size()/2, [](auto& a){a *= 4;}):"
         << endl;
    vector<int> v7{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    std::for_each_n(std::execution::par_unseq, std::begin(v7), v7.size() / 2,
                    [](auto& a) { a *= 4; });
    cout << "v: { ";
    for (auto& e : v7) {
        cout << e << " ";
    }
    cout << "}" << endl;

    cout << endl;
    cout << "std::ranges::for_each_n(std::begin(v), v.size()/2, Functor{}):"
         << endl;
    vector<int> v8{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto result3 =
        (std::ranges::for_each_n(std::begin(v8), v8.size() / 2, Functor{})).fun;
    cout << "{result.cnt; result.sum} = {" << result3.cnt << "; " << result3.sum
         << "}" << endl;
};
}  // namespace functional
