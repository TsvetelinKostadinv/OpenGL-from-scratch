#pragma once

#include <cassert>
#include <vector>
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

#include "gl_types.h"
#include "util/typedef.h"
#include "util/vec.h"

// Buffer is the storage of the data
// Could be extended into buffer array
class gpu_buffer
{
public:
    gpu_buffer() noexcept { glGenBuffers(1, &id); }
    gpu_buffer(gpu_buffer&& other) noexcept : id(other.id) { other.id = 0; };
    gpu_buffer& operator=(gpu_buffer&& other) noexcept
    {
        this->~gpu_buffer();
        id = other.id;
        other.id = 0;
    }

    ~gpu_buffer() { glDeleteBuffers(1, &id); }

    gpu_buffer(const gpu_buffer&) = delete;
    gpu_buffer& operator=(const gpu_buffer&) = delete;

    GLuint getId() const { return id; }
    void bind(GLenum bufferType) const { glBindBuffer(bufferType, id); }

    void loadData(GLenum bufferType,
                  u32 sizeInBytes,
                  void* data,
                  GLenum usage) const
    {
        glBindBuffer(bufferType, id);
        glBufferData(bufferType, sizeInBytes, data, usage);
    }

private:
    GLuint id;
};

// Defines how the data is layed out in a buffer
class layout
{
public:
    template <typename T>
    void push(u32 size = 1, bool normalized = false)
    {
        elements.emplace_back(layout_element{
            u32(elements.size()), size, gl_type<T>::gl_variant, normalized});
        stride += size * getSizeOfGLtype(gl_type<T>::gl_variant);
    }

    template <>
    void push<vec3f>(u32 size, bool normalized)
    {
        for (u32 i = 0; i < size; ++i)
        {
            push<float>(3, normalized);
        }
    }

#pragma warning(disable : 4312)  // because of the const void* cast
    void enable() const
    {
        u32 currentOffset = 0;
        for (const layout_element& el : elements)
        {
            glEnableVertexAttribArray(el.index);
            glVertexAttribPointer(el.index, el.size, el.type,
                                  el.normalized ? GL_TRUE : GL_FALSE, stride,
                                  (const void*) currentOffset);
            currentOffset += el.size * getSizeOfGLtype(el.type);
        }
    }
#pragma warning(default : 4312)

    u32 sizeOfVertex() const { return stride; }

private:
    struct layout_element
    {
        u32 index;
        u32 size;
        GLenum type;
        bool normalized;
    };

    u32 stride = 0;
    std::vector<layout_element> elements;

    static u32 getSizeOfGLtype(GLenum type) noexcept
    {
        switch (type)
        {
            case GL_FLOAT:
                return sizeof(float);
            case GL_FLOAT_VEC2:
                return 2 * sizeof(float);
            case GL_FLOAT_VEC3:
                return 3 * sizeof(float);
            case GL_FLOAT_VEC4:
                return 4 * sizeof(float);
            default:
                assert(false && "Unknown GL type");
                return 0;
        }
    }
};

// The actual vertex array - knows how to interpret the data
class vertex_array
{
public:
    vertex_array(const gpu_buffer& vertBuffer,
                 const layout& layout,
                 const gpu_buffer* indBuf = nullptr)
        : vertBuf(vertBuffer), indBuf(indBuf), layout(layout)
    {
        glGenVertexArrays(1, &id);
        glBindVertexArray(id);
        layout.enable();
    }

    void enable() const
    {
        glBindVertexArray(id);
        vertBuf.bind(GL_ARRAY_BUFFER);
        if (indBuf != nullptr)
        {
            indBuf->bind(GL_ELEMENT_ARRAY_BUFFER);
        }
    }

    i32 countToDraw() const
    {
        i32 res = 0;
        if (indBuf != nullptr)
        {
            indBuf->bind(GL_ELEMENT_ARRAY_BUFFER);
            glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE,
                                   &res);
            return res / sizeof(unsigned int);
        }
        else
        {
            vertBuf.bind(GL_ARRAY_BUFFER);
            glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &res);
            return res / layout.sizeOfVertex();
        }
    }

private:
    u32 id;
    const gpu_buffer& vertBuf;
    const gpu_buffer* indBuf;
    layout layout;
};