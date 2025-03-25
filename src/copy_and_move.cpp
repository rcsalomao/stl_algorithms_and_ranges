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

namespace copy_and_move {
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

struct CopyOnly {
    CopyOnly() { cout << "CopyOnly constructor" << endl; };
    CopyOnly(const CopyOnly&) = default;
    CopyOnly& operator=(const CopyOnly&) {
        cout << "Copy assignment!" << endl;
        return *this;
    }
    // A criação implícita das operações de movimentação ('move constructor' e
    // 'move assignment') é impedida por meio da definição explícita das
    // operações de cópia na classe.
};

std::string to_string(const CopyOnly&) { return "{CopyOnly obj}"; };

void main() {
    {
        cout << endl;
        cout << "std::ranges::copy(v, std::back_inserter(o)):" << endl;
        auto v = vw::iota(1, 5) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        vector<int> o;
        rg::copy(v, std::back_inserter(o));
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::copy(std::ranges::subrange(v.begin()+1, "
                "v.begin()+3), std::back_inserter(o)):"
             << endl;
        auto v = vw::iota(1, 5) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        vector<int> o;
        rg::copy(rg::subrange(v.begin() + 1, v.begin() + 3),
                 std::back_inserter(o));
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "std::copy(v.begin(), v.begin()+2, v.begin()+2):" << endl;
        auto v = vw::iota(1, 5) | rg::to<vector<int>>();
        cout << "original 'v': " << stringify(v) << endl;
        std::copy(v.begin(), v.begin() + 2, v.begin() + 2);
        cout << "modified 'v': " << stringify(v) << endl;
    };
    {
        cout << endl;
        cout << "std::move(v.begin(), v.begin()+2, v.begin()+2):" << endl;
        auto v = vw::iota(1, 5) |
                 vw::transform([](int) { return CopyOnly{}; }) |
                 rg::to<vector<CopyOnly>>();
        cout << "original 'v': " << stringify(v) << endl;
        std::move(v.begin(), v.begin() + 2, v.begin() + 2);
        cout << "modified 'v': " << stringify(v) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::copy_backward(v, std::end(o)):" << endl;
        auto v = vw::iota(1, 6) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        std::vector<int> o(v.size() + 2);
        std::ranges::copy_backward(v, std::end(o));
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "std::copy_backward(v.begin(), std::prev(v.end(), 2), v.end()):"
             << endl;
        auto v = vw::iota(1, 6) | rg::to<vector<int>>();
        cout << "original 'v': " << stringify(v) << endl;
        std::copy_backward(v.begin(), std::prev(v.end(), 2), v.end());
        cout << "modified 'v': " << stringify(v) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::copy_n(std::begin(v) + 1, 3, "
                "std::back_inserter(o)):"
             << endl;
        auto v = vw::iota(1, 6) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        vector<int> o;
        std::ranges::copy_n(std::begin(v) + 1, 3, std::back_inserter(o));
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::copy_if(v, std::back_inserter(o), [](int i) { "
                "return i % 2 == 0; }):"
             << endl;
        auto v = vw::iota(1, 6) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        vector<int> o;
        std::ranges::copy_if(v, std::back_inserter(o),
                             [](int i) { return i % 2 == 0; });
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::remove_copy_if(v, std::back_inserter(o), [](int "
                "i) { return i % 2 == 0; }):"
             << endl;
        auto v = vw::iota(1, 6) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        vector<int> o;
        std::ranges::remove_copy_if(v, std::back_inserter(o),
                                    [](int i) { return i % 2 == 0; });
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::remove_copy(v, std::back_inserter(o), 3):"
             << endl;
        auto v = vw::iota(1, 6) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        vector<int> o;
        std::ranges::remove_copy(v, std::back_inserter(o), 3);
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::sample(v, std::back_inserter(o), 3, "
                "std::random_device{})"
             << endl;
        auto v = vw::iota(1, 6) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        vector<int> o;
        std::ranges::sample(v, std::back_inserter(o), 3, std::random_device{});
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::replace_copy(v, std::back_inserter(o), 3, 42):"
             << endl;
        auto v = vw::iota(1, 6) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        vector<int> o;
        std::ranges::replace_copy(v, std::back_inserter(o), 3, 42);
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::replace_copy_if(v, std::back_inserter(o), "
                "[](int i) { return i % 2 == 0; }, 42):"
             << endl;
        auto v = vw::iota(1, 6) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        vector<int> o;
        std::ranges::replace_copy_if(
            v, std::back_inserter(o), [](int i) { return i % 2 == 0; }, 42);
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::reverse_copy(v, std::back_inserter(o)):" << endl;
        auto v = vw::iota(1, 6) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        vector<int> o;
        std::ranges::reverse_copy(v, std::back_inserter(o));
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::rotate_copy(v, std::begin(v) + 3, "
                "std::back_inserter(o)):"
             << endl;
        auto v = vw::iota(1, 6) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        vector<int> o;
        std::ranges::rotate_copy(v, std::begin(v) + 3, std::back_inserter(o));
        cout << "'o': " << stringify(o) << endl;
    };
};
}  // namespace copy_and_move
