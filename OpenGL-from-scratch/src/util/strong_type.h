#pragma once
#include <type_traits>
#include <utility>

// code is inspired by the lecture:
// Strong Types in C++ - Barney Dellar, C++ on Sea 2019
// modified with added assurances
// link: https://www.youtube.com/watch?v=fWcnp7Bulc8

template <typename Underlying, template <typename> typename Current>
struct crtp
{
    constexpr const Underlying& underlying() const
    {
        return static_cast<const Underlying&>(*this);
    }
};

// uses the predefined + operator of the strong type's base type
template <typename Underlying>
struct forwarding_addable : crtp<Underlying, forwarding_addable>
{
    constexpr Underlying add(const Underlying& other) const
    {
        return Underlying(this->underlying().value + other.value);
    }

    constexpr Underlying operator+(const Underlying& other) const
    {
        return Underlying(this->underlying().value + other.value);
    }
};

template <typename T,
          typename Identifier,
          template <typename>
          typename... Decorators>
struct strong_type
    : public Decorators<strong_type<T, Identifier, Decorators...>>...
{
    template <
        typename = std::enable_if_t<std::is_default_constructible<T>::value>>
    constexpr explicit strong_type() noexcept(
        std::is_nothrow_default_constructible<T>::value)
        : Decorators<strong_type<T, Identifier, Decorators...>>()..., value()
    {
    }

    template <typename = std::enable_if_t<std::is_copy_constructible<T>::value>>
    constexpr explicit strong_type(const T& value) noexcept(
        std::is_nothrow_copy_constructible<T>::value)
        : value(value),
    {
    }

    template <typename = std::enable_if_t<std::is_copy_constructible<T>::value>>
    constexpr explicit strong_type(const strong_type& other) noexcept(
        std::is_nothrow_copy_constructible<T>::value)
        : value(other.value)
    {
    }

    // assignment is not preferrable as it introduces conversions
    strong_type& operator=(const T& value)
    {
        static_assert(false, "Cannot assign the base type to a strong type!");
    }

    template <typename = std::enable_if_t<std::is_copy_assignable<T>::value>>
    constexpr strong_type operator=(const strong_type& other) noexcept(
        std::is_nothrow_copy_assignable<T>::value)
    {
        value = other.value;
        return *this;
    }

    template <typename = std::enable_if_t<std::is_move_constructible<T>::value>>
    constexpr explicit strong_type(T&& value) noexcept(
        std::is_nothrow_move_constructible<T>::value)
        : value(std::move(value))
    {
    }

    template <typename = std::enable_if_t<std::is_move_constructible<T>::value>>
    constexpr explicit strong_type(strong_type&& other) noexcept(
        std::is_nothrow_move_constructible<T>::value)
        : value(std::move(other.value))
    {
    }

    // assignment is not preferrable as it introduces conversions
    strong_type& operator=(T&& value)
    {
        static_assert(false, "Cannot move the base type into a strong type!");
    }

    template <typename = std::enable_if_t<std::is_move_assignable<T>::value>>
    constexpr strong_type& operator=(strong_type&& other) noexcept(
        std::is_nothrow_move_assignable<T>::value)
    {
        value = std::move(other.value);
        return *this;
    }

    constexpr explicit operator T() const noexcept { return value; }

    constexpr T& operator->() { return value; }
    constexpr const T& operator->() const { return value; }

    // contained on the stack like a usual variable
    T value;
};
