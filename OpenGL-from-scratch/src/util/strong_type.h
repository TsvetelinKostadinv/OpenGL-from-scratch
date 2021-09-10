#pragma once
#include <type_traits>
#include "typedef.h"

template <typename T>
struct strong_type
{
    constexpr explicit strong_type() noexcept(
        std::is_trivially_constructible_v<T>)
        : value()
    {
    }

    constexpr explicit strong_type(const T& value) noexcept(
        std::is_nothrow_copy_constructible_v<T>)
        : value(value){};

    constexpr explicit strong_type(T&& value) noexcept(
        std::is_nothrow_move_constructible_v<T>)
        : value(std::move(value)){};

    constexpr explicit strong_type(const strong_type& other) noexcept(
        std::is_nothrow_copy_constructible_v<T>)
        : value(other.value)
    {
    }

    constexpr explicit strong_type(strong_type&& other) noexcept(
        std::is_nothrow_move_constructible_v<T>)
        : value(std::move(other.value))
    {
    }

    constexpr explicit operator T() const noexcept { return value; }

    constexpr T& operator->() { return value; }
    constexpr const T& operator->() const { return value; }

    // contained on the stack like a usual variable
    T value;
};