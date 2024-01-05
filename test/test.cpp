#include <cstddef>
#include <iostream>
#include <mini_mpf/type_array.hpp>
#include <mini_mpf/type_umap.hpp>
#include <string>
#include <type_traits>

using namespace mini_mpf;

template <typename T>
struct tra_arr {
    void operator()(double n)
    {
        if constexpr (std::is_same_v<T, int>)
            std::cout << "traverse arr with given arguments: int" << n << "\n";
        if constexpr (std::is_same_v<T, double>)
            std::cout << "traverse arr with given arguments: double" << n << "\n";
        if constexpr (std::is_same_v<T, float&>)
            std::cout << "traverse arr with given arguments: float&" << n << "\n";
    }
};

template <typename T, std::size_t N>
struct tra_arr_i {
    void operator()()
    {
        if constexpr (N == 0)
            std::cout << "traverse arr with index: int" << N << "\n";
        if constexpr (N == 1)
            std::cout << "traverse arr with index: double" << N << "\n";
        if constexpr (N == 2)
            std::cout << "traverse arr with index: float&" << N << "\n";
    }
};

static void test_array()
{
    using arr = type_array<int, double, float&>;

    // get a type from arr
    static_assert(std::is_same_v<float&, arr::at<2>>, "bad");

    // get the number of types in arr
    static_assert(3 == arr::len(), "bad");

    // compare a given type with the type at Pos
    static_assert(arr::cmp<float&, 2>(), "bad");

    // forward all types in arr to tuple
    using tuple = std::tuple<int, double, float&>;
    static_assert(std::is_same_v<tuple, arr::forward<std::tuple>>, "bad");

    // determine whether a given type exists in arr
    static_assert(arr::find<float&>() == 2, "bad");
    // actually the below find will throw an assertion failure during compile time 
    // static_assert(arr::find<double&>() == -1, "bad");

    // find_if will never throw any assertion
    static_assert(!arr::find_if<float>(), "bad");

    // traverse all types in arr
    arr::for_each<tra_arr>(1.0);
    arr::for_each<tra_arr_i>();
}

template <typename T>
struct tra_umap {
    void operator()(double n)
    {
        if constexpr (std::is_same_v<T, int>)
            std::cout << "traverse umap with given arguments: int" << n << "\n";
        if constexpr (std::is_same_v<T, float>)
            std::cout << "traverse umap with given arguments: float" << n << "\n";
        if constexpr (std::is_same_v<T, double>)
            std::cout << "traverse arr with given arguments: double" << n << "\n";
    }
};

enum class Num {
    i32,
    f32,
    f64
};

template <typename T, Num K>
struct tra_umap_k {
    void operator()()
    {
        if constexpr (K == Num::i32)
            std::cout << "traverse umap with key: i32 int" << "\n";
        if constexpr (K == Num::f32)
            std::cout << "traverse umap with key: f32 float" << "\n";
        if constexpr (K == Num::f64)
            std::cout << "traverse umap with key: f64 double" << "\n";
    }
};

static void test_umap()
{
    using umap = type_umap<Num, int, float, double>;
    using arr  = umap::array;
    using umap2 = type_umap<Num, arr>;

    // get a type from umap
    static_assert(std::is_same_v<float, umap::at<Num::f32>>, "bad");

    // get the number of types in umap
    static_assert(3 == umap::len(), "bad");

    // compare a given type with the type at Pos
    static_assert(umap::cmp<float, Num::f32>(), "bad");

    // forward all types in umap to tuple
    using tuple = std::tuple<int, float, double>;
    static_assert(std::is_same_v<tuple, umap::forward<std::tuple>>, "bad");
    
    // determine whether a given type exists in umap
    static_assert(umap::find<float>() == Num::f32, "bad");
    static_assert(umap2::find<double>() == Num::f64, "bad");
    static_assert(!umap2::find_if<float&>(), "bad");

    // traverse all types in umap
    umap::for_each<tra_umap>(1.0);
    umap::for_each<tra_umap_k>();
}

int main()
{
    test_array();
    test_umap();
}