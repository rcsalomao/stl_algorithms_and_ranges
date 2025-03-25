#include <algorithm>
#include <boost/type_index.hpp>
#include <iomanip>
#include <iostream>
#include <list>
#include <print>
#include <random>
#include <ranges>
#include <unordered_map>
#include <vector>

namespace ranges_and_views {
using boost::typeindex::type_id_with_cvr;
using std::cout;
using std::endl;
using std::print;
using std::println;
using std::string;
using std::vector;
namespace rg = std::ranges;
namespace vw = std::views;

// ADL
using std::to_string;
std::string_view to_string(std::string_view s) { return s; };
string to_string(const std::pair<const int, double>& pair) {
    auto [k, v] = pair;
    return "{" + to_string(k) + ", " + to_string(v) + "}";
}
string to_string(const std::tuple<int, int, string>& tup) {
    auto [i1, i2, s] = tup;
    return "{" + to_string(i1) + ", " + to_string(i2) + ", " + s + "}";
}

template <std::ranges::forward_range Rng>
auto stringify(Rng&& seq) {
    auto b = seq | vw::transform([](const auto& a) { return to_string(a); }) |
             vw::join_with(',') | vw::common;
    return "{" + std::string(std::begin(b), std::end(b)) + "}";
}

struct Foo {
    int x{0};
};

string to_string(const Foo& f) { return "{" + to_string(f.x) + "}"; }

void main() {
    {
        cout << endl;
        cout << "v = std::views::iota(1) | std::views::take(5) | "
                "std::ranges::to<std::vector<int>>():"
             << endl;
        vector<int> v =
            vw::iota(1) | vw::take(5) | std::ranges::to<vector<int>>();
        auto it1 = v.begin();
        auto it2 = v.end();
        cout << "'v': {";
        for (auto& e : rg::subrange(it1, std::prev(it2))) {
            cout << e << ",";
        }
        cout << *std::prev(it2) << "}" << endl;
    };
    {
        cout << endl;
        cout << "v = std::views::iota(1, 7)  |  "
                "std::ranges::to<std::vector<int>>():"
             << endl;
        vector<int> v = vw::iota(1, 7) | std::ranges::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        auto it = std::begin(v);
        cout << "'[v[0], v[0]+4)': {";
        std::ranges::for_each(it, it + 3, [](auto& e) { cout << e << ","; });
        cout << *(it + 3) << "}" << endl;
    };
    {
        cout << endl;
        std::list l{1, 2, 3, 4, 5, 6, 7};
        cout << "'l': " << stringify(l) << endl;
        auto it = std::begin(l);
        cout << "it = list.begin();" << endl;
        cout << "*it: " << *it << endl;
        // it += 5;  // erro: 'forward iterator' não possui random access,
        //           // e portanto não possui o operador '+='.
        it++;                   // ok: todos os iteradores implementam '++'.
        it = std::next(it, 2);  // ok
        std::advance(it, 2);    // ok
        cout << "it++; it = std::next(it, 2); std::advance(it, 2);" << endl;
        cout << "*it: " << *it << endl;
        // auto dist =
        //     l.end() - it;  // erro: 'forward iterator' não possui a
        //                    // implementação para diferença de iteradores.
        auto dist = std::distance(it, std::end(l));
        cout << "dist = std::distance(it, std::end(l));" << endl;
        cout << "dist: " << dist << endl;
    };
    {
        cout << endl;
        cout << "v = std::views::iota(13, 19) | std::views::transform([](auto "
                "i) { return Foo{i}; }) | std::ranges::to<vector<Foo>>()"
             << endl;
        vector<Foo> v = vw::iota(13, 19) |
                        vw::transform([](auto i) { return Foo{i}; }) |
                        std::ranges::to<vector<Foo>>();
        cout << "'v': " << stringify(v) << endl;
    };
    {
        //  uma coisa em se ter em mente é sobre 'dangling references'. No caso,
        //  deve-se tomar cuidado ao utilizar variáveis temporárias como
        //  argumentos de entrada para determinados algoritmos:
        cout << endl;
        const char* c_str = "19340857";
        cout << "'c_str': " << c_str << endl;
        auto it1 = rg::find(std::string_view(c_str),
                            '0');  // ok. 'string_view' é uma 'borrowed range'.
        cout << "it = std::ranges::find(std::string_view(c_str), '0');" << endl;
        cout << "'*it': " << *it1
             << " na posição: " << std::distance(c_str, it1) << endl;
        int ruim = 1987501;
        auto it2 = rg::find(std::to_string(ruim),
                            '0');  // decltype(it2) == std::ranges::dangling.
                                   // erro de compilação com '*it2'.
        // cout << *it2 << endl;      // indirection requires pointer operand
        //                            // ('borrowed_iterator_t<basic_string<char
        //                            // ,char_traits<char>, allocator<char>>>'
        //                            // (aka 'std::ranges::dangling') invalid)
    };
    {
        // 'views' são 'stateful', ou seja, possuem estado próprio e portanto,
        // deve-se tomar cuidado com a invalidação do 'cache' interno ao se
        // trabalhar com elas.
        cout << endl;
        cout << "!'views' são 'stateful':" << endl;
        auto v = vw::iota(0, 9) | rg::to<std::list<int>>();
        cout << "'v': " << stringify(v) << endl;

        auto view = v | vw::drop(4);
        cout << "view = v | std::views::drop(4);" << endl;

        cout << "'view': ";
        for (auto& a : view) cout << a;
        cout << endl;

        v.push_front(42);
        cout << "v.push_front(42);" << endl;

        cout << "'view': ";
        for (auto& a : view) cout << a;
        cout << endl;

        cout << "for (auto& a: v | std::views::drop(4)) { cout << a }: ";
        for (auto& a : v | vw::drop(4)) cout << a;
        cout << endl;
    };
    {
        cout << endl;
        println("'std::views::keys' e 'std::views::values':");
        auto umap = vw::iota(0, 4) | vw::transform([](int i) {
                        // return std::pair<const int, double>{i, i * 0.5};
                        return std::make_pair(i, i * 0.5);
                    }) |
                    rg::to<std::unordered_map<int, double>>();
        println("std::unordered_map<int, double> 'umap': {}", stringify(umap));

        vector<int> keys;
        rg::copy(vw::keys(umap), std::back_inserter(keys));
        println("std::views::keys('umap'): {}", stringify(keys));

        vector<double> values;
        rg::copy(vw::values(umap), std::back_inserter(values));
        println("std::views::values('umap'): {}", stringify(values));
    };
    {
        cout << endl;
        vector<string> vs{"Cat", "Dog", "Car"};
        auto data = vw::iota(0, 3) | vw::transform([&vs](int i) {
                        return std::make_tuple(i, 100 - i, vs[i]);
                    }) |
                    rg::to<std::vector<std::tuple<int, int, string>>>();
        println("std::vector<std::tuple<int, int, string>> 'data': {}",
                stringify(data));

        vector<int> second;
        rg::copy(vw::elements<1>(data), std::back_inserter(second));
        println("std::views::elements<1>('data'): {}", stringify(second));

        vector<string> third;
        rg::copy(data | vw::elements<2>, std::back_inserter(third));
        println("std::views::elements<2>('data'): {}", stringify(third));
    };
    {
        cout << endl;
        auto v = vw::iota(0, 9) | rg::to<std::vector<int>>();
        rg::shuffle(v, std::random_device{});
        println("'v': {}", stringify(v));

        vector<int> o1;
        rg::copy(v | vw::take(4), std::back_inserter(o1));
        println("'v' | std::views::take(4): {}", stringify(o1));

        vector<int> o2;
        rg::copy(v | vw::take_while([](int a) { return a > 1; }),
                 std::back_inserter(o2));
        cout << "'v' | std::views::take_while([](int a){return a>1;}): "
             << stringify(o2) << endl;
    };
    {
        cout << endl;
        auto v = vw::iota(0, 9) | rg::to<std::vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "'v': " << stringify(v) << endl;

        vector<int> o1;
        rg::copy(v | vw::drop(4), std::back_inserter(o1));
        cout << "'v' | std::views::drop(4): " << stringify(o1) << endl;

        vector<int> o2;
        rg::copy(v | vw::drop_while([](auto& a) { return a > 1; }),
                 std::back_inserter(o2));
        cout << "'v' | std::views::drop_while([](auto& a){return a>1;}): "
             << stringify(o2) << endl;
    };
    {
        cout << endl;
        auto v = vw::iota(0, 9) | rg::to<std::vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "'v': " << stringify(v) << endl;
        cout << "'v' | std::views::filter([](auto& a){return a%2==0;}): "
             << stringify(v | vw::filter([](auto& a) { return a % 2 == 0; }))
             << endl;
    };
    {
        cout << endl;
        auto v = vw::iota(0, 9) | rg::to<std::vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "'v': " << stringify(v) << endl;
        cout << "'v' | std::views::reverse: " << stringify(v | vw::reverse)
             << endl;
    };
    {
        cout << endl;
        auto v = vw::iota(0, 9) | rg::to<std::vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "'v': " << stringify(v) << endl;
        cout << "'v' | std::views::counted(v.begin() + 3, 3): "
             << stringify(vw::counted(v.begin() + 3, 3)) << endl;
    };
    {
        cout << endl;
        auto v = vw::iota(0, 9) |
                 vw::transform([](int v) { return to_string(v); }) |
                 vw::join_with(',') | vw::common;
        cout << "'v' = std::views::iota(0, 9) | std::views::transform([](int "
                "v) { return to_string(v); }) | std::views::join_with(',') | "
                "std::views::common;"
             << endl;
        cout << "std::string(v.begin(), v.end()): "
             << string(v.begin(), v.end())
             << endl;  // os iteradores 'v.begin()' e 'v.end()'
                       // precisam ser exatamente do mesmo tipo.
    };
    {
        cout << endl;
        auto v = vw::iota(0, 9) | rg::to<vector<int>>();
        rg::shuffle(v, std::random_device{});
        cout << "'v': " << stringify(v) << endl;
        cout << "std::views::all('v'): " << stringify(vw::all(v)) << endl;
    };
    {
        cout << endl;
        string s = "1.23.13.42";
        cout << "'s': " << s << endl;
        vector<int> o;
        cout << "std::vector<int> o;" << endl;
        rg::copy(s | vw::split('.') | vw::transform([](auto a) {
                     // return std::stoi(string(a.begin(), a.end()));
                     int res = 0;
                     std::from_chars(a.data(), a.data() + a.size(), res);
                     return res;
                 }),
                 std::back_inserter(o));
        cout << "std::ranges::copy(s | std::views::split('.') | "
                "std::views::transform([](auto a) { int res = 0; "
                "std::from_chars(a.data(), a.data() + a.size(), res); return "
                "res; }), std::back_inserter(o));"
             << endl;
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        string s = "Car Dog Window Frog";
        cout << "'s': " << s << endl;
        vector<string> o;
        cout << "std::vector<std::string> o;" << endl;
        rg::for_each(s | vw::lazy_split(' '), [&o](const auto& a) {
            auto common = a | vw::common;
            o.emplace_back(common.begin(), common.end());
        });
        cout << "std::ranges::for_each(s | std::views::lazy_split(' '), "
                "[&o](const auto& a) { auto common = a | std::views::common; "
                "o.emplace_back(common.begin(), common.end()); });"
             << endl;
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        string s = "Carros Lanchas Jetskis Motos";
        cout << "'s': " << s << endl;
        auto j1 = s | vw::split(' ') | vw::join | vw::common;
        cout << "j1 = s | std::views::split(' ') | std::views::join | "
                "std::views::common;"
             << endl;
        cout << "string(j1.begin(), j1.end()): " << string(j1.begin(), j1.end())
             << endl;
        auto j2 = s | vw::split(' ') | vw::join_with(':') | vw::common;
        cout << "j2 = s | std::views::split(' ') | std::views::join_with(':') "
                "| std::views::common;"
             << endl;
        cout << "string(j2.begin(), j2.end()): " << string(j2.begin(), j2.end())
             << endl;
    };
    {
        cout << endl;
        vector<int> o;
        cout << "std::vector<int> o;" << endl;
        rg::copy(vw::empty<int>, std::back_inserter(o));
        cout << "std::ranges::copy(std::views::empty<int>, "
                "std::back_inserter(o));"
             << endl;
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        vector<int> o;
        cout << "std::vector<int> o;" << endl;
        rg::copy(vw::single(42), std::back_inserter(o));
        cout << "std::ranges::copy(std::views::single(42), "
                "std::back_inserter(o));"
             << endl;
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        vector<int> o;
        cout << "std::vector<int> o;" << endl;
        rg::copy(vw::iota(2, 8), std::back_inserter(o));
        cout << "std::ranges::copy(vw::iota(2, 8), std::back_inserter(o));"
             << endl;
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        vector<int> o;
        cout << "std::vector<int> o;" << endl;
        rg::copy(vw::iota(42) | vw::take(6), std::back_inserter(o));
        cout << "std::ranges::copy(std::views::iota(42) | std::views::take(6), "
                "std::back_inserter(o));"
             << endl;
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        std::istringstream n{"1 -2 24 -42 99 82"};
        cout << "std::istringstream n{\"1 -2 24 -42 99 82\"};" << endl;
        cout << "std::views::istream<int>(n) | std::views::filter([](int "
                "i){return i>0;}) | std::ranges::to<std::vector<int>>(): "
             << stringify(vw::istream<int>(n) |
                          vw::filter([](int i) { return i > 0; }) |
                          rg::to<vector<int>>())
             << endl;
    };
    {
        cout << endl;
        auto f = std::istringstream{"1.1  2.2\t3.3\v4.4\f55\n66\r7.7  8.8"};
        cout << "f = std::istringstream{\"1.1  2.2\\t3.3\\v4.4\\f55\\n66\\r7.7 "
                " 8.8\"}"
             << endl;
        cout << "std::views::istream<double>(f) | std::views::filter([](double "
                "i){return i>3.3;}) | std::ranges::to<std::vector<double>>(): "
             << stringify(vw::istream<double>(f) |
                          vw::filter([](double i) { return i > 3.3; }) |
                          rg::to<vector<double>>())
             << endl;
    };
};
}  // namespace ranges_and_views
