#pragma once

#include "GLFW/glfw3.h"

template <typename T>
struct gl_type
{
};

template <>
struct gl_type<float>
{
    constexpr static GLenum type = GL_FLOAT;
};

template <>
struct gl_type<double>
{
    constexpr static GLenum type = GL_DOUBLE;
};

template <>
struct gl_type<char>
{
    constexpr static GLenum type = GL_BYTE;
};
template <>
struct gl_type<unsigned char>
{
    constexpr static GLenum type = GL_UNSIGNED_BYTE;
};

template <>
struct gl_type<short>
{
    constexpr static GLenum type = GL_SHORT;
};

template <>
struct gl_type<unsigned short>
{
    constexpr static GLenum type = GL_UNSIGNED_SHORT;
};

template <>
struct gl_type<int>
{
    constexpr static GLenum type = GL_INT;
};

template <>
struct gl_type<unsigned int>
{
    constexpr static GLenum type = GL_UNSIGNED_INT;
};