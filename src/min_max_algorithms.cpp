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

namespace min_max_algorithms {
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

struct X {
    static int copy_count;
    static int move_count;
    X(int v) : value{v} {};
    X(const X& other) : value(other.value) { ++copy_count; }
    X& operator=(const X& other) {
        value = other.value;
        ++copy_count;
        return *this;
    }
    X(X&& other) : value(std::move(other.value)) { ++move_count; }
    X& operator=(X&& other) {
        value = std::move(other.value);
        ++move_count;
        return *this;
    }
    int value;
    friend auto operator<=>(const X&, const X&) = default;
};

string to_string(const X& x) { return "{" + std::to_string(x.value) + "}"; };

int X::copy_count = 0;
int X::move_count = 0;

void main() {
    {
        cout << endl;
        cout << "Infelizmente os algoritmos 'std::max', 'std::min', "
                "'std::minmax' e 'std::clamp' sofrem com problemas de cópias "
                "excessivas dos respectivos valores e 'dangling references' "
                "devido ao fato de serem funções que aceitam como argumentos "
                "de entrada 'const ref' e retornam 'const ref'."
             << endl;
        cout << "Desta forma, não irei perder meu tempo estudando estas "
                "funções. Já que entendo que é mais vantajoso implementar "
                "manualmente estas funcionalidades."
             << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::min_element(v):" << endl;
        vector<X> v{{2}, {-2}, {42}, {24}};
        cout << "'v': " << stringify(v) << endl;
        auto i = std::ranges::min_element(v);
        cout << "'res': " << to_string(*i)
             << " na posição: " << std::distance(v.begin(), i) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::max_element(v):" << endl;
        vector<X> v{{2}, {-2}, {42}, {24}};
        cout << "'v': " << stringify(v) << endl;
        auto i = std::ranges::max_element(v);
        cout << "'res': " << to_string(*i)
             << " na posição: " << std::distance(v.begin(), i) << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::minmax_element(v):" << endl;
        vector<X> v{{2}, {-2}, {42}, {24}};
        // cout << "'v': " << stringify(v) << endl;
        cout << "'v': " << stringify(std::span(v.begin(), v.size())) << endl;
        auto [min, max] = std::ranges::minmax_element(v);
        cout << "'min res': " << to_string(*min)
             << " na posição: " << std::distance(v.begin(), min) << endl;
        cout << "'max res': " << to_string(*max)
             << " na posição: " << std::distance(v.begin(), max) << endl;
    };
};
}  // namespace min_max_algorithms
