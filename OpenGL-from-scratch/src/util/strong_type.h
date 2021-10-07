#pragma once
#include <ratio>
#include <type_traits>

#include "typedef.h"

template <typename T, typename Identifier, typename... Decorators>
struct strong_type
{
    template <typename = std::enable_if_t<std::is_default_constructible_v<T>>>
    constexpr explicit strong_type() noexcept(
        std::is_nothrow_default_constructible_v<T>)
        : value()
    {
    }

    template <typename = std::enable_if_t<std::is_copy_constructible_v<T>>>
    constexpr explicit strong_type(const T& value) noexcept(
        std::is_nothrow_copy_constructible_v<T>)
        : value(value)
    {
    }

    template <typename = std::enable_if_t<std::is_copy_constructible_v<T>>>
    constexpr explicit strong_type(const strong_type& other) noexcept(
        std::is_nothrow_copy_constructible_v<T>)
        : value(other.value)
    {
    }

    template <typename = std::enable_if_t<std::is_move_constructible_v<T>>>
    constexpr explicit strong_type(T&& value) noexcept(
        std::is_nothrow_move_constructible_v<T>)
        : value(std::forward(value))
    {
    }

    template <typename = std::enable_if_t<std::is_move_constructible_v<T>>>
    constexpr explicit strong_type(strong_type&& other) noexcept(
        std::is_nothrow_move_constructible_v<T>)
        : value(std::forward(other.value))
    {
    }

    constexpr explicit operator T() const noexcept { return value; }

    constexpr T& operator->() { return value; }
    constexpr const T& operator->() const { return value; }

    // contained on the stack like a usual variable
    T value;
};

template <typename T, typename Identifier, typename... Decorators>
using primitive_type = strong_type<T, Identifier, Decorators...>;