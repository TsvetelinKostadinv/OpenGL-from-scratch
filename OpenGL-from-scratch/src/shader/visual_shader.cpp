#include "visual_shader.h"

std::optional<visual_shader> visual_shader::makeShader(
    std::ostream& errStream,
    const char* vertSource,
    const char* fragSource,
    const char* geomSource) noexcept

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
        errStream << "[ERROR] Program linking failed:\n" << log << std::endl;
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
        res.emplace(vertexShader, fragmentShader, std::nullopt, shaderProgram);
    }
    return res;
}

std::optional<visual_shader> visual_shader::makeShaderFromFiles(
    std::ostream& errStream,
    const std::filesystem::path vertexShaderSourcePath,
    const std::filesystem::path fragShaderSourcePath,
    std::optional<const std::filesystem::path> geomShaderSourcePath)

{
    std::ifstream in(vertexShaderSourcePath);
    std::string vertShaderSource;
    in.seekg(0, std::ios::end);
    vertShaderSource.reserve(in.tellg());
    in.seekg(0, std::ios::beg);

    vertShaderSource.assign((std::istreambuf_iterator<char>(in)),
                            std::istreambuf_iterator<char>());

    in.close();
    in.open(fragShaderSourcePath);
    std::string fragShaderSource;
    in.seekg(0, std::ios::end);
    fragShaderSource.reserve(in.tellg());
    in.seekg(0, std::ios::beg);
    fragShaderSource.assign((std::istreambuf_iterator<char>(in)),
                            std::istreambuf_iterator<char>());

    if (geomShaderSourcePath.has_value())
    {
        in.close();
        in.open(geomShaderSourcePath.value());
        std::string geomShaderSource;
        in.seekg(0, std::ios::end);
        geomShaderSource.reserve(in.tellg());
        in.seekg(0, std::ios::beg);
        geomShaderSource.assign((std::istreambuf_iterator<char>(in)),
                                std::istreambuf_iterator<char>());
        return visual_shader::makeShader(errStream, vertShaderSource.c_str(),
                                         fragShaderSource.c_str(),
                                         geomShaderSource.c_str());
    }
    else
    {
        return visual_shader::makeShader(errStream, vertShaderSource.c_str(),
                                         fragShaderSource.c_str());
    }
}

visual_shader::visual_shader(visual_shader&& other) noexcept
    : vertShaderId(other.vertShaderId),
      fragShaderId(other.fragShaderId),
      geomShaderId(other.geomShaderId),
      programId(other.programId),
      activeAttributesCount(other.activeAttributesCount),
      activeUniformsCount(other.activeUniformsCount),
      uniformsCache(std::move(other.uniformsCache))
{
    other.vertShaderId = 0;
    other.fragShaderId = 0;
    other.geomShaderId = 0;
    other.programId = 0;
    other.activeAttributesCount = 0;
    other.activeUniformsCount = 0;
}

visual_shader& visual_shader::operator=(visual_shader&& other) noexcept
{
    this->~visual_shader();
    vertShaderId = other.vertShaderId;
    fragShaderId = other.vertShaderId;
    if (other.geomShaderId.has_value())
    {
        geomShaderId = other.geomShaderId.value();
    }
    else
    {
        geomShaderId = {};
    }

    programId = other.programId;
    activeAttributesCount = other.activeAttributesCount;
    activeUniformsCount = other.activeUniformsCount;

    std::exchange(uniformsCache, std::move(other.uniformsCache));

    return *this;
}

visual_shader::~visual_shader()
{
    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderId);
    if (geomShaderId.has_value())
    {
        glDeleteShader(geomShaderId.value());
    }
    glDeleteProgram(programId);
}

GLint visual_shader::getGeometryInputType() const noexcept
{
    assert(hasGeometryShader());
    GLint inputType;
    glGetProgramiv(programId, GL_GEOMETRY_INPUT_TYPE, &inputType);
    return inputType;
}

visual_shader::visual_shader(unsigned int vertShaderId,
                             unsigned int fragShaderId,
                             std::optional<unsigned int> geomShaderId,
                             unsigned int programId)

    : vertShaderId(vertShaderId),
      fragShaderId(fragShaderId),
      geomShaderId(geomShaderId),
      programId(programId)
{
    glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &activeAttributesCount);
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &activeUniformsCount);

    for (int i = 0; i < activeUniformsCount; i++)
    {
        constexpr u32 bufferSize = 128;
        i32 length;
        i32 size;
        GLenum type;
        char name[bufferSize];
        glGetActiveUniform(programId, i, bufferSize, &length, &size, &type,
                           name);

        uniformsCache[name] = i;
    }
}
