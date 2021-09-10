#pragma once

#include <cstddef>
#include <utility>

template <std::size_t I>
struct everything
{
    template <typename T>
    constexpr operator T&() const;
};

template <class T, std::size_t I0, std::size_t... I>
constexpr auto getFieldsCount(std::size_t& out, std::index_sequence<I0, I...>)
    -> decltype(T{everything<I0>{}, everything<I>{}...})*
{
    static_assert(std::is_aggregate<T>, "The type must be an aggregate");
    out = sizeof...(I) + 1;
    return nullptr;
}

template <class T, std::size_t... I>
constexpr auto getFieldsCount(std::size_t& out, std::index_sequence<I...>)
{
    static_assert(std::is_aggregate<T>, "The type must be an aggregate");
    getFieldsCount<T>(out, std::make_index_sequence<sizeof...(I) - 1>{});
}

template <typename T>
constexpr std::size_t getFieldsCount()
{
    static_assert(std::is_aggregate<T>, "The type must be an aggregate");
    std::size_t count;
    getFieldsCount<T>(count, std::make_index_sequence<sizeof(T)>{});
    return count;
}
