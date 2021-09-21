#pragma once

#include <assert.h>
#include <optional>
#include <ostream>

#include "glad/glad.h"
#include "util/typedef.h"

class visual_shader
{
public:
    static std::optional<visual_shader> makeShader(
        std::ostream& errStream,
        const char* vertSource,
        const char* fragSource,
        const char* geomSource = nullptr) noexcept
    {
        assert(vertSource != nullptr &&
               "You have to supply vertex source - it was nullptr");
        assert(fragSource != nullptr &&
               "You have to supply fragment source - it was nullptr");
        int status;
        constexpr u32 logLen = 512;
        char log[logLen];

        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertSource, NULL);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

        if (!status)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, log);
            errStream << "[ERROR] Vertex shader compilation failed:\n"
                      << log << std::endl;
            return std::nullopt;
        }

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, log);
            errStream << "[ERROR] Fragment shader compilation failed:\n"
                      << log << std::endl;
            return std::nullopt;
        }

        unsigned int geomShader = 0;
        if (geomSource != nullptr)
        {
            geomShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geomShader, 1, &geomSource, NULL);
            glCompileShader(geomShader);

            glGetShaderiv(geomShader, GL_COMPILE_STATUS, &status);
            if (!status)
            {
                glGetShaderInfoLog(geomShader, 512, NULL, log);
                errStream << "[ERROR] Geometry shader compilation failed:\n"
                          << log << std::endl;
                return std::nullopt;
            }
        }

        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        if (geomSource != nullptr)
        {
            glAttachShader(shaderProgram, geomShader);
        }

        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
        if (!status)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, log);
            errStream << "[ERROR] Program linking failed:\n"
                      << log << std::endl;
            return std::nullopt;
        }

        std::optional<visual_shader> res = std::nullopt;
        if (geomSource != nullptr)
        {
            res.emplace(vertexShader, fragmentShader, std::optional(geomShader),
                        shaderProgram);
        }
        else
        {
            res.emplace(vertexShader, fragmentShader, std::nullopt,
                        shaderProgram);
        }
        return res;
    }

    visual_shader(visual_shader&& other) noexcept
        : vertShaderId(other.vertShaderId),
          fragShaderId(other.fragShaderId),
          geomShaderId(other.geomShaderId),
          programId(other.programId)
    {
        other.vertShaderId = 0;
        other.fragShaderId = 0;
        other.geomShaderId = 0;
        other.programId = 0;
    }

    ~visual_shader()
    {
        glDeleteShader(vertShaderId);
        glDeleteShader(fragShaderId);
        if (geomShaderId.has_value())
        {
            glDeleteShader(geomShaderId.value());
        }
        glDeleteProgram(programId);
    }

    bool hasGeometryShader() const noexcept { return geomShaderId.has_value(); }

    unsigned int getGeometryInputType() const noexcept
    {
        assert(hasGeometryShader());
        int inputType;
        glGetProgramiv(programId, GL_GEOMETRY_INPUT_TYPE, &inputType);
        return inputType;
    }

    void useProgram() const { glUseProgram(programId); }

public:
    unsigned int vertShaderId;
    unsigned int fragShaderId;
    std::optional<unsigned int> geomShaderId;
    unsigned int programId;

    // does not perform any validation
    visual_shader(unsigned int vertShaderId,
                  unsigned int fragShaderId,
                  std::optional<unsigned int> geomShaderId,
                  unsigned int programId)
        : vertShaderId(vertShaderId),
          fragShaderId(fragShaderId),
          geomShaderId(geomShaderId),
          programId(programId)
    {
    }

    template <class _Ty>
    friend class std::optional;
};