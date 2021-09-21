#pragma once
#include <cassert>
#include <cmath>
#include "typedef.h"

struct vec3f
{
    // this is for the use of unnamed struct in a union which is not standard
#pragma warning(disable : 4201)
    union
    {
        float f[3];
        struct
        {
            float x, y, z;
        };
    };
#pragma warning(default : 4201)

    constexpr explicit vec3f() noexcept : f{0, 0, 0} {}
    constexpr explicit vec3f(float val) noexcept : f{val, val, val} {}
    constexpr vec3f(float x, float y, float z) noexcept : f{x, y, z} {}

    constexpr vec3f(const vec3f& other) noexcept
        : x(other.x), y(other.y), z(other.z)
    {
    }

    constexpr vec3f& operator=(const vec3f& other) noexcept
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    constexpr static float dot(const vec3f& a, const vec3f b) noexcept
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    constexpr static vec3f cross(const vec3f& a, const vec3f b) noexcept
    {
        return vec3f{a.y * b.z - a.z * b.y,  //
                     a.z * b.x - a.x * b.z,  //
                     a.x * b.y - a.y * b.x};
    }

    constexpr float lengthSquare() const noexcept { return dot(*this, *this); }

    // cannot be noexcept because of std::sqrt
    float length() const noexcept { return std::sqrt(dot(*this, *this)); }

    constexpr vec3f operator+=(const vec3f& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    constexpr vec3f operator-=(const vec3f& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    constexpr vec3f operator*=(const float c) noexcept
    {
        x *= c;
        y *= c;
        z *= c;
        return *this;
    }

    constexpr float operator[](i32 i) const noexcept
    {
        assert(i < 3 && "The index has to be in the range [0;3]");
        return f[i];
    }

    constexpr float& operator[](i32 i) noexcept
    {
        assert(i < 3 && "The index has to be in the range [0;3]");
        return f[i];
    }

    // mixed product, also the determinant of the matrix with these vectors as
    // rows/columns
    constexpr static float mixed(const vec3f& fst,
                                 const vec3f& scd,
                                 const vec3f& trd)
    {
        return fst[0] * scd[1] * trd[2] + fst[1] * scd[2] * trd[0] +
               scd[0] * trd[1] * fst[2] - fst[2] * scd[1] * trd[0] -
               fst[1] * scd[0] * trd[2] - scd[2] * trd[1] * fst[0];
    }
};

constexpr vec3f operator+(const vec3f& a, const vec3f& b) noexcept
{
    return vec3f{a.x + b.x, a.y + b.y, a.z + b.z};
}

// dot product
constexpr float operator*(const vec3f& a, const vec3f& b) noexcept
{
    return vec3f::dot(a, b);
}

constexpr vec3f operator*(const float c, const vec3f& a) noexcept
{
    return vec3f{a.x * c, a.y * c, a.z * c};
}

constexpr vec3f operator*(const vec3f& a, const float c) noexcept
{
    return c * a;
}

constexpr vec3f operator-(const vec3f& a, const vec3f& b) noexcept
{
    return vec3f{a.x - b.x, a.y - b.y, a.z - b.z};
}

constexpr vec3f operator/(const vec3f& a, const float c) noexcept
{
    return (1.0f / c) * a;
}

// cross product
constexpr vec3f operator^(const vec3f& a, const vec3f& b) noexcept
{
    return vec3f::cross(a, b);
}