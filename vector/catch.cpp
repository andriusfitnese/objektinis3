#define CATCH_CONFIG_RUNNER
#include "catch_amalgamated.hpp"
#include "vector.hpp"
#include <sstream>
#include <string>
#include <type_traits>

TEST_CASE("Vector default‐constructs empty", "[Vector][ctor]") {
    Vector<int> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.empty());
    REQUIRE(v.capacity() == 0);
}

TEST_CASE("Vector fill‐constructor and element access", "[Vector][ctor]") {
    Vector<std::string> v(3, "foo");
    REQUIRE(v.size() == 3);
    for (size_t i = 0; i < 3; ++i) {
        REQUIRE(v[i] == "foo");
        REQUIRE(v.at(i) == "foo");
    }
    REQUIRE_THROWS_AS(v.at(3), std::out_of_range);
}

TEST_CASE("push_back, pop_back, resize", "[Vector][mod]") {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    REQUIRE(v.size() == 3);
    REQUIRE(v.back() == 3);

    v.pop_back();
    REQUIRE(v.size() == 2);
    REQUIRE(v.back() == 2);

    v.resize(5, 7);
    REQUIRE(v.size() == 5);
    REQUIRE(v[4] == 7);

    v.resize(1);
    REQUIRE(v.size() == 1);
    REQUIRE(v.front() == 1);
}

TEST_CASE("reserve and capacity growth", "[Vector][capacity]") {
    Vector<int> v;
    v.reserve(10);
    REQUIRE(v.capacity() >= 10);
    REQUIRE(v.size() == 0);

    for (int i = 0; i < 10; ++i)
        v.push_back(i);
    REQUIRE(v.size() == 10);
    REQUIRE(v.capacity() >= 10);
}

TEST_CASE("shrink_to_fit releases extra capacity", "[Vector][capacity]") {
    Vector<int> v(100, 42);
    REQUIRE(v.capacity() >= 100);
    v.resize(10);
    v.shrink_to_fit();
    REQUIRE(v.capacity() == 10);
}

TEST_CASE("iterators and range‐for", "[Vector][iter]") {
    Vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i);
    int expected = 0;
    for (auto& x : v) {
        REQUIRE(x == expected++);
    }
    REQUIRE(expected == 5);
}

TEST_CASE("non‐member swap and relational operators", "[Vector][swap][rel]") {
    Vector<int> a = { 1,2,3 };
    Vector<int> b = { 1,2,4 };
    REQUIRE((a < b));
    swap(a, b);
    REQUIRE(a.front() == 1);
    REQUIRE(a.back() == 4);
}

TEST_CASE("range assign", "[Vector][assign]") {
    std::vector<int> src = { 10, 20, 30 };
    Vector<int> v;
    v.assign(src.begin(), src.end());
    REQUIRE(v.size() == src.size());
    REQUIRE(std::equal(v.begin(), v.end(), src.begin()));
}

template<typename T, typename Alloc>
void require_equal(const Vector<T, Alloc>& a, const Vector<T, Alloc>& b) {
    REQUIRE(a.size() == b.size());
    REQUIRE(a.capacity() == b.capacity());
    REQUIRE(std::equal(a.begin(), a.end(), b.begin()));
}

TEST_CASE("Vector Copy Constructor", "[Vector][copy][ctor]") {
    Vector<int> orig = { 1, 2, 3, 4, 5 };
    orig.reserve(10);

    Vector<int> copy(orig);  // copy construct
    require_equal(orig, copy);

    REQUIRE(orig.size() == 5);
    REQUIRE(orig.capacity() >= 10);
}

TEST_CASE("Vector Copy Assignment", "[Vector][copy][assign]") {
    Vector<std::string> a = { "foo", "bar" };
    a.reserve(8);

    Vector<std::string> b;
    b = a;  // copy assign
    require_equal(a, b);

    a = a;
    REQUIRE(a.size() == 2);
    REQUIRE(a[0] == "foo");
}

TEST_CASE("Vector Move Constructor", "[Vector][move][ctor]") {
    Vector<int> orig = { 10, 20, 30 };
    orig.reserve(16);

    Vector<int> moved(std::move(orig));  // move ctor

    REQUIRE((orig.size() == 0 || orig.capacity() == 0));
    REQUIRE(moved.size() == 3);
    REQUIRE(moved.capacity() >= 16);
    REQUIRE(moved[0] == 10);
    REQUIRE(moved[2] == 30);
}

TEST_CASE("Vector Move Assignment", "[Vector][move][assign]") {
    Vector<int> a = { 7, 8, 9 };
    a.reserve(12);

    Vector<int> b;
    b = std::move(a);  // move assign

    REQUIRE((a.size() == 0 || a.capacity() == 0));
    REQUIRE(b.size() == 3);
    REQUIRE(b.capacity() >= 12);
    REQUIRE(b[1] == 8);

    b = std::move(b);
    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == 7);
}

TEST_CASE("Vector noexcept move operations", "[Vector][move][noexcept]") {
    STATIC_REQUIRE(std::is_nothrow_move_constructible<Vector<int>>::value);
    STATIC_REQUIRE(std::is_nothrow_move_assignable<Vector<int>>::value);
}



TEST_CASE("stream IO", "[Vector][io]") {
    Vector<int> v = { 5,6,7 };
    std::ostringstream oss;
    oss << v;
    REQUIRE(oss.str() == "[5, 6, 7]");

    Vector<int> w;
    std::istringstream iss("[8, 9, 10]");
    iss >> w;
    REQUIRE(w.size() == 3);
    REQUIRE(w[0] == 8);
    REQUIRE(w[2] == 10);
}

TEST_CASE("max_size sanity check", "[Vector][capacity]") {
    Vector<int> v;
    STATIC_REQUIRE(std::is_same<decltype(v.max_size()), size_t>::value);
    REQUIRE(v.max_size() > 0);
}