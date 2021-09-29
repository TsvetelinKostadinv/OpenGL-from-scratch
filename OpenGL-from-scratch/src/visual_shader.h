#pragma once

#include <assert.h>
#include <filesystem>
#include <fstream>
#include <optional>
#include <ostream>
#include <unordered_map>

#include "glad/glad.h"
#include "util/typedef.h"
#include "util/vec.h"

class visual_shader
{
public:
    visual_shader(visual_shader&& other) noexcept;
    visual_shader& operator=(visual_shader&& other);

    visual_shader(const visual_shader&) = delete;
    visual_shader& operator=(const visual_shader&) = delete;

    ~visual_shader();

    static std::optional<visual_shader> makeShader(
        std::ostream& errStream,
        const char* vertSource,
        const char* fragSource,
        const char* geomSource = nullptr) noexcept;

    static std::optional<visual_shader> makeShaderFromFiles(
        std::ostream& errStream,
        const std::filesystem::path vertexShaderSourcePath,
        const std::filesystem::path fragShaderSourcePath,
        std::optional<const std::filesystem::path> geomShaderSourcePath);

    bool hasGeometryShader() const noexcept { return geomShaderId.has_value(); }

    GLint getGeometryInputType() const noexcept;

    int getActiveAttributesCount() const { return activeAttributesCount; }

    int getActiveUniformsCount() const { return activeUniformsCount; }

    void useProgram() const { glUseProgram(programId); }

    void setUniform1f(const std::string& name, float val) const
    {
        glUniform1f(uniformsCache.at(name), val);
    }

    void setUniform2f(const std::string& name, float val1, float val2) const
    {
        glUniform2f(uniformsCache.at(name), val1, val2);
    }

    void setUniform3f(const std::string& name,
                      float val1,
                      float val2,
                      float val3) const
    {
        glUniform3f(uniformsCache.at(name), val1, val2, val3);
    }

    void setUniform3f(const std::string& name, const vec3f& values) const
    {
        glUniform3f(uniformsCache.at(name), values.x, values.y, values.z);
    }

    void setUniform4f(const std::string& name,
                      float val1,
                      float val2,
                      float val3,
                      float val4) const
    {
        glUniform4f(uniformsCache.at(name), val1, val2, val3, val4);
    }

public:
    // does not perform any validation
    // just extracts data
    visual_shader(unsigned int vertShaderId,
                  unsigned int fragShaderId,
                  std::optional<unsigned int> geomShaderId,
                  unsigned int programId);

    template <class _Ty>
    friend class std::optional;

private:
    unsigned int vertShaderId;
    unsigned int fragShaderId;
    std::optional<unsigned int> geomShaderId;
    unsigned int programId;

    int activeAttributesCount;
    int activeUniformsCount;

    std::unordered_map<std::string, i32> uniformsCache;
};
