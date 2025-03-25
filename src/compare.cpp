#include <algorithm>
#include <boost/type_index.hpp>
#include <execution>
#include <iostream>
#include <print>
#include <random>
#include <ranges>
#include <vector>

namespace compare {
using boost::typeindex::type_id_with_cvr;
using std::cout;
using std::endl;
using std::vector;

struct Point {
    int x;
    int y;

    // pre-C++20 operador '<' lexicográfico
    friend bool operator<(const Point& left, const Point& right) {
        if (left.x != right.x) {
            return left.x < right.x;
        }
        return left.y < right.y;
    };

    // // operador 'spaceship' padrão de C++20
    // friend auto operator<=>(const Point&, const Point&) = default;

    // operador 'spaceship' próprio definido
    friend auto operator<=>(const Point& left, const Point& right) {
        if (left.x != right.x) {
            return left.x < right.x;
        }
        return left.y < right.y;
    };
};

void main() {
    // no caso de tipos definidos, para que estes sejam comparáveis e
    // ordenáveis, a 'stl' exige que estes obedeçam ao requisito de
    // 'strict_weak_ordering':
    // - irreflexividade: ¬f(a,a)
    // - antisimetria: f(a,b) ⇒ ¬f(b,a)
    // - transitividade: (f(a,b)^f(b,c)) ⇒ f(a,c)

    // Uma boa implementação padrão de 'strict_weak_ordering' é o ordenamento
    // 'lexicográfico', também disponibilizado pelos containeres padrão.
    // Desde C++20 o operador 'spaceship' permite que tipos definidos possam
    // acessar a versão padrão do ordenamento 'lexicográfico' (ver struct
    // 'Point').

    // O ordenamento 'strict_weak_ordering' para ranges é exposto por meio do
    // algoritmo std::lexicographical_compare:
    cout << endl;
    vector<int> v1{1, 2, 3};
    vector<int> v2{1, 3};
    cout << "std::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), "
            "v2.end()):"
         << endl;
    bool cmp1 = std::lexicographical_compare(v1.begin(), v1.end(), v2.begin(),
                                             v2.end());
    cout << std::boolalpha << cmp1 << endl;

    cout << endl;
    cout << "std::ranges::lexicographical_compare(v2, v1):" << endl;
    bool cmp2 = std::ranges::lexicographical_compare(v2, v1);
    cout << std::boolalpha << cmp2 << endl;

    cout << endl;
    cout << "v1 < v2:" << endl;
    bool cmp3 = v1 < v2;
    cout << std::boolalpha << cmp3 << endl;

    cout << endl;
    vector<std::string> names1{"Zod", "Celeste"};
    vector<std::string> names2{"Adam", "Maria"};
    cout << "std::ranges::lexicographical_compare(names1, names2, [](const "
            "auto& l, const auto& r) { return l.length() < r.length(); }):"
         << endl;
    bool cmp4 = std::ranges::lexicographical_compare(
        names1, names2,
        [](const auto& l, const auto& r) { return l.length() < r.length(); });
    cout << std::boolalpha << cmp4 << endl;

    cout << endl;
    cout << "std::ranges::lexicographical_compare(names1, names2):" << endl;
    bool cmp5 = std::ranges::lexicographical_compare(names1, names2);
    cout << std::boolalpha << cmp5 << endl;

    cout << endl;
    cout << "std::ranges::lexicographical_compare(names1, names2, "
            "std::greater{}, [](auto& a){return a.length();}, [](auto& "
            "a){return a.length();}):"
         << endl;
    bool cmp6 = std::ranges::lexicographical_compare(
        names1, names2, std::greater<>{}, [](auto& a) { return a.length(); },
        [](auto& a) { return a.length(); });
    cout << std::boolalpha << cmp6 << endl;

    cout << endl;
    // 'lexicographical_compare_three_way' é o equivalente ao operador
    // 'spaceship' para 'lexicographical_compare' e deve retornar um dos tipos:
    // - 'std::strong_ordering'
    // - 'std::weak_ordering'
    // - 'std::partial_ordering'
    vector<int> v3{1, 1, 1};
    vector<int> v4{1, 2, 3};
    cout << "std::ranges::lexicographical_compare_three_way(v3.begin(), "
            "v3.end(), v4.begin(), v4.end()):"
         << endl;
    auto cmp7 = std::lexicographical_compare_three_way(v3.begin(), v3.end(),
                                                       v4.begin(), v4.end());
    cout << type_id_with_cvr<decltype(cmp7)>().pretty_name() << endl;
    cout << std::is_lt(cmp7) << endl;
};
}  // namespace compare
