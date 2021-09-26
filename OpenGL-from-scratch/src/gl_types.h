#pragma once

#include "GLFW/glfw3.h"

template <typename T>
struct gl_type
{
    using cpp_cariant = T;
};

template <>
struct gl_type<float>
{
    constexpr static GLenum gl_variant = GL_FLOAT;
};

template <>
struct gl_type<double>
{
    constexpr static GLenum gl_variant = GL_DOUBLE;
};

template <>
struct gl_type<char>
{
    constexpr static GLenum gl_variant = GL_BYTE;
};
template <>
struct gl_type<unsigned char>
{
    constexpr static GLenum gl_variant = GL_UNSIGNED_BYTE;
};

template <>
struct gl_type<short>
{
    constexpr static GLenum gl_variant = GL_SHORT;
};

template <>
struct gl_type<unsigned short>
{
    constexpr static GLenum gl_variant = GL_UNSIGNED_SHORT;
};

template <>
struct gl_type<int>
{
    constexpr static GLenum gl_variant = GL_INT;
};

template <>
struct gl_type<unsigned int>
{
    constexpr static GLenum gl_variant = GL_UNSIGNED_INT;
};