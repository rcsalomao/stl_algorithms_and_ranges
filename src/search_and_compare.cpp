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

namespace search_and_compare {
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

void main() {
    {
        cout << endl;
        cout << "std::ranges::find(v, 3):" << endl;
        auto v = vw::iota(1, 5) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        auto it = rg::find(v, 3);
        if (it != v.end()) {
            cout << "Found value: " << *it
                 << " on position: " << std::distance(v.begin(), it) << endl;
        }
    };
    // {
    //     cout << endl;
    //     cout << "(...)\nfind(it, s.end(), ';')\n(...):" << endl;
    //     string s = "João;Maria;Pedro;Diógenes;";
    //     char delim = ';';
    //     cout << "'s': " << s << endl;
    //     auto it = s.begin();
    //     vector<string> o;
    //     while (true) {
    //         auto token = std::find(it, s.end(), delim);
    //         if (token != s.end()) {
    //             o.push_back(std::string(it, token));
    //             it = std::next(token);
    //         } else {
    //             break;
    //         }
    //     }
    //     cout << "'o': " << stringify(o) << endl;
    // };
    {
        cout << endl;
        cout << "(...)\nfind(it, s.end(), ';')\n(...):" << endl;
        string s = "João;Maria;Pedro;Diógenes;";
        cout << "'s': " << s << endl;
        auto it = s.begin();
        auto token = s.begin();
        vector<string> o;
        while ((token = find(it, s.end(), ';')) != s.end()) {
            o.push_back("");
            std::copy(it, token, std::back_inserter(o.back()));
            it = std::next(token);
        }
        cout << "'o': " << stringify(o) << endl;
    };
    {
        cout << endl;
        cout << "word_begin = std::find_if_not( s.begin(), s.end(), [](char c) "
                "{ return std::isspace(c); });\nword_end = std::find_if_not( "
                "s.rbegin(), s.rend(), [](char c) { return std::isspace(c); "
                "});\no{word_begin, word_end.base()}:"
             << endl;
        string s = "         Hello World!!     ";
        auto word_begin = std::find_if_not(
            s.begin(), s.end(), [](char c) { return std::isspace(c); });
        auto word_end = std::find_if_not(
            s.rbegin(), s.rend(), [](char c) { return std::isspace(c); });
        string o{word_begin, word_end.base()};
        // string o;
        // std::copy(word_begin, word_end.base(), std::back_inserter(o));
        cout << "'s': " << s << endl;
        cout << "'o': " << o << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::adjacent_find(v):" << endl;
        vector<int> v = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        cout << "'v': " << stringify(v) << endl;
        auto it = std::ranges::adjacent_find(v);
        if (it != v.end()) {
            cout << "'res': " << "{" << *it << ", " << *std::next(it) << "}"
                 << endl;
        }
    };
    {
        cout << endl;
        cout << "std::ranges::adjacent_find(v, [](int l, int r) { return l + r "
                "> 5; }):"
             << endl;
        auto v = vw::iota(1, 5) | rg::to<vector<int>>();
        cout << "'v': " << stringify(v) << endl;
        auto it = rg::adjacent_find(v, [](int l, int r) { return l + r > 5; });
        if (it != v.end()) {
            cout << "'res': " << "{" << *it << ", " << *std::next(it) << "}"
                 << endl;
        }
    };
    {
        cout << endl;
        cout << "std::ranges::search_n(v, 2, 3):" << endl;
        vector<int> v = {1, 0, 5, 8, 3, 3, 3, 2};
        cout << "'v': " << stringify(v) << endl;
        auto [first, last] = rg::search_n(v, 2, 3);
        if (first != v.end()) {
            cout << "'res': " << stringify(rg::subrange(first, last)) << endl;
        }
    };
    {
        cout << endl;
        cout << "std::ranges::search_n(v, 3, 3, [](int l, int r) { return l % "
                "5 == r % 5; }):"
             << endl;
        vector<int> v = {1, 0, 5, 8, 3, 3, 3, 2};
        cout << "'v': " << stringify(v) << endl;
        auto [first, last] =
            rg::search_n(v, 3, 3, [](int l, int r) { return l % 5 == r % 5; });
        if (first != v.end()) {
            cout << "'res': " << stringify(rg::subrange(first, last)) << endl;
        }
    };
    {
        cout << endl;
        cout << "std::ranges::find_first_of(v, w):" << endl;
        vector<int> haystack = vw::iota(1, 10) | rg::to<vector<int>>();
        vector<int> needles = {7, 5, 3};
        cout << "'v': " << stringify(haystack) << endl;
        cout << "'w': " << stringify(needles) << endl;
        auto it = rg::find_first_of(haystack, needles);
        if (it != haystack.end()) {
            cout << "'res': " << *it
                 << " na posição: " << std::distance(haystack.begin(), it)
                 << endl;
        }
    };
    {
        cout << endl;
        cout << "std::ranges::search(v, w):" << endl;
        string haystack = "abbabba";
        string needle = "bba";
        cout << "'v': " << haystack << endl;
        cout << "'w': " << needle << endl;
        auto [first, last] = rg::search(haystack, needle);
        if (first != haystack.end()) {
            cout << "'res': " << string{first, last} << " na posição: " << "["
                 << std::distance(haystack.begin(), first) << ", "
                 << std::distance(haystack.begin(), last) << ")" << endl;
        }
    };
    {
        cout << endl;
        cout << "std::ranges::find_end(v, w):" << endl;
        string haystack = "abbabba";
        string needle = "bba";
        cout << "'v': " << haystack << endl;
        cout << "'w': " << needle << endl;
        auto [first, last] = rg::find_end(haystack, needle);
        if (first != haystack.end()) {
            cout << "'res': " << string{first, last} << " na posição: " << "["
                 << std::distance(haystack.begin(), first) << ", "
                 << std::distance(haystack.begin(), last) << ")" << endl;
        }
    };
    {
        cout << endl;
        cout << "std::search((...), std::'searcher'(...)):" << endl;
        string haystack = "abbabba";
        string needle = "bba";
        cout << "'v': " << haystack << endl;
        cout << "'w': " << needle << endl;
        cout << "std::search(v.begin(), v.end(), "
                "std::default_searcher(w.begin(), w.end())):"
             << endl;
        auto first0 =
            std::search(haystack.begin(), haystack.end(),
                        std::default_searcher(needle.begin(), needle.end()));
        auto last0 = std::next(first0, needle.size());
        if (first0 != haystack.end()) {
            cout << "'res0': " << string{first0, last0}
                 << " na posição: " << "["
                 << std::distance(haystack.begin(), first0) << ", "
                 << std::distance(haystack.begin(), last0) << ")" << endl;
        }
        cout << "std::search(v.begin(), v.end(), "
                "std::boyer_moore_searcher(w.begin(), w.end())):"
             << endl;
        auto first1 = std::search(
            haystack.begin(), haystack.end(),
            std::boyer_moore_searcher(needle.begin(), needle.end()));
        auto last1 = std::next(first1, needle.size());
        if (first1 != haystack.end()) {
            cout << "'res1': " << string{first1, last1}
                 << " na posição: " << "["
                 << std::distance(haystack.begin(), first1) << ", "
                 << std::distance(haystack.begin(), last1) << ")" << endl;
        }
        cout << "std::search(v.begin(), v.end(), "
                "std::boyer_moore_horspool_searcher(w.begin(), w.end())):"
             << endl;
        auto first2 = std::search(
            haystack.begin(), haystack.end(),
            std::boyer_moore_horspool_searcher(needle.begin(), needle.end()));
        auto last2 = std::next(first2, needle.size());
        if (first2 != haystack.end()) {
            cout << "'res2': " << string{first2, last2}
                 << " na posição: " << "["
                 << std::distance(haystack.begin(), first2) << ", "
                 << std::distance(haystack.begin(), last2) << ")" << endl;
        }
    };
    {
        cout << endl;
        cout << "std::ranges::count(v, 3):" << endl;
        vector<int> v = {1, 0, 5, 8, 3, 3, 3, 2};
        cout << "'v': " << stringify(v) << endl;
        long count1 = rg::count(v, 3);
        cout << "'res': " << count1 << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::count_if(v, [](int i) { return i > 3; }):"
             << endl;
        vector<int> v = {1, 0, 5, 8, 3, 3, 3, 2};
        cout << "'v': " << stringify(v) << endl;
        long count2 = rg::count_if(v, [](int i) { return i > 3; });
        cout << "'res': " << count2 << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::equal(v, w):" << endl;
        vector<int> v = {1, 0, 5, 8, 3, 3, 3, 2};
        vector<int> w = {-1, 0, -5, -8, -3, -3, -3, -2};
        cout << "'v': " << stringify(v) << endl;
        cout << "'w': " << stringify(w) << endl;
        auto test1 = rg::equal(v, w);
        cout << "'res': " << std::boolalpha << test1 << endl;
    };
    {
        cout << endl;
        cout << "std::ranges::equal(v, w, [](int l, int r) { return "
                "std::abs(l) == std::abs(r); }):"
             << endl;
        vector<int> v = {1, 0, 5, 8, 3, 3, 3, 2};
        vector<int> w = {-1, 0, -5, -8, -3, -3, -3, -2};
        cout << "'v': " << stringify(v) << endl;
        cout << "'w': " << stringify(w) << endl;
        auto test2 = rg::equal(
            v, w, [](int l, int r) { return std::abs(l) == std::abs(r); });
        cout << "'res': " << std::boolalpha << test2 << endl;
    };
    {
        cout << endl;
        cout << "std::mismatch(v.begin(), v.end(), w.begin()):" << endl;
        vector<int> v = {1, 0, 5, 8, 3, 3, 3, 2};
        vector<int> w = {1, 0, 5, 9, 1, 3, 3, 2};
        cout << "'v': " << stringify(v) << endl;
        cout << "'w': " << stringify(w) << endl;
        auto [first1, second1] = std::mismatch(v.begin(), v.end(), w.begin());
        cout << "'res': " << "{" << *first1 << ", " << *second1 << "}" << endl;
    };
    {
        cout << endl;
        cout << "std::mismatch(v.begin(), v.end(), w.begin(), [](int l, int r) "
                "{ return l - r <= 0; }):"
             << endl;
        vector<int> v = {1, 0, 5, 8, 3, 3, 3, 2};
        vector<int> w = {1, 0, 5, 9, 1, 3, 3, 2};
        cout << "'v': " << stringify(v) << endl;
        cout << "'w': " << stringify(w) << endl;
        auto [first2, second2] =
            std::mismatch(v.begin(), v.end(), w.begin(),
                          [](int l, int r) { return l - r <= 0; });
        cout << "'res': " << "{" << *first2 << ", " << *second2 << "}" << endl;
    };
};
}  // namespace search_and_compare
