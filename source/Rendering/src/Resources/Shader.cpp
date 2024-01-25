#include "Resources/Shader.h"

#include <sstream>

#include <Debug/Assertion.h>
#include <Debug/Log.h>

#include <glad/glad.h>

#include <Utility/utility.h>

using namespace LibGL::Utility;

namespace LibGL::Rendering::Resources
{
    REGISTER_RESOURCE_TYPE(Shader);

    Shader::Shader(const Shader& other) :
        IResource(other), m_source(other.m_source)
    {
        if (other.m_program != 0)
        ASSERT(parseSource());
    }

    Shader::Shader(Shader&& other) noexcept :
        m_source(std::move(other.m_source)), m_program(other.m_program)
    {
        other.m_program = 0;
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_program);
    }

    Shader& Shader::operator=(const Shader& other)
    {
        if (&other == this)
            return *this;

        m_source = other.m_source;

        if (other.m_program != 0)
        ASSERT(parseSource());

        return *this;
    }

    Shader& Shader::operator=(Shader&& other) noexcept
    {
        if (&other == this)
            return *this;

        m_source = other.m_source;
        m_program = other.m_program;

        other.m_program = 0;

        return *this;
    }

    bool Shader::load(const char* fileName)
    {
        const std::ifstream fs(fileName);

        if (!fs.is_open())
        {
            DEBUG_LOG("Unable to open file at path \"%s\"\n", fileName);
            return false;
        }

        std::ostringstream stringStream;

        stringStream << fs.rdbuf();

        m_source = stringStream.str();
        return true;
    }

    bool Shader::init()
    {
        if (parseSource())
            return true;

        DEBUG_LOG("Unable to initialize shader - Couldn't pare source");
        return false;
    }

    void Shader::use() const
    {
        glUseProgram(m_program);
    }

    void Shader::unbind()
    {
        glUseProgram(0);
    }

    void Shader::setUniformInt(const std::string& name, const int value)
    {
        glUniform1i(getUniformLocation(name), value);
    }

    void Shader::setUniformFloat(const std::string& name, const float value)
    {
        glUniform1f(getUniformLocation(name), value);
    }

    void Shader::setUniformVec2(const std::string& name, const LibMath::Vector2& value)
    {
        glUniform2fv(getUniformLocation(name), 1, value.getArray());
    }

    void Shader::setUniformVec3(const std::string& name, const LibMath::Vector3& value)
    {
        glUniform3fv(getUniformLocation(name), 1, value.getArray());
    }

    void Shader::setUniformVec4(const std::string& name, const LibMath::Vector4& value)
    {
        glUniform4fv(getUniformLocation(name), 1, value.getArray());
    }

    void Shader::setUniformMat4(const std::string& name, const LibMath::Matrix4& value)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, value.getArray());
    }

    int Shader::getUniformInt(const std::string& name)
    {
        int value;
        glGetUniformiv(m_program, getUniformLocation(name), &value);
        return value;
    }

    float Shader::getUniformFloat(const std::string& name)
    {
        float value;
        glGetUniformfv(m_program, getUniformLocation(name), &value);
        return value;
    }

    LibMath::Vector2 Shader::getUniformVec2(const std::string& name)
    {
        GLfloat values[16];
        glGetUniformfv(m_program, getUniformLocation(name), values);
        return reinterpret_cast<LibMath::Vector2&>(values);
    }

    LibMath::Vector3 Shader::getUniformVec3(const std::string& name)
    {
        GLfloat values[16];
        glGetUniformfv(m_program, getUniformLocation(name), values);
        return reinterpret_cast<LibMath::Vector3&>(values);
    }

    LibMath::Vector4 Shader::getUniformVec4(const std::string& name)
    {
        GLfloat values[16];
        glGetUniformfv(m_program, getUniformLocation(name), values);
        return reinterpret_cast<LibMath::Vector4&>(values);
    }

    LibMath::Matrix4 Shader::getUniformMat4(const std::string& name)
    {
        GLfloat values[16];
        glGetUniformfv(m_program, getUniformLocation(name), values);
        return reinterpret_cast<LibMath::Matrix4&>(values);
    }

    std::string Shader::getTokenFromType(const uint32_t shaderType)
    {
        switch (shaderType)
        {
        case GL_VERTEX_SHADER:
            return "vertex";
        case GL_FRAGMENT_SHADER:
            return "fragment";
        case GL_GEOMETRY_SHADER:
            return "geometry";
        case GL_TESS_EVALUATION_SHADER:
            return "tess_evaluation";
        case GL_TESS_CONTROL_SHADER:
            return "tess_control";
        case GL_COMPUTE_SHADER:
            return "compute";
        default:
            return {};
        }
    }

    uint32_t Shader::getTypeFromToken(const std::string& shaderType)
    {
        if (shaderType == "vertex")
            return GL_VERTEX_SHADER;

        if (shaderType == "fragment")
            return GL_FRAGMENT_SHADER;

        if (shaderType == "geometry")
            return GL_GEOMETRY_SHADER;

        if (shaderType == "tess_evaluation")
            return GL_TESS_EVALUATION_SHADER;

        if (shaderType == "tess_control")
            return GL_TESS_CONTROL_SHADER;

        if (shaderType == "compute")
            return GL_COMPUTE_SHADER;

        return GL_INVALID_VALUE;
    }

    bool Shader::processIncludes(std::string& source)
    {
        if (source.empty())
            return true;

        std::string        line;
        std::istringstream sourceStream(source);

        while (std::getline(sourceStream, line))
        {
            if (!line.starts_with("#include "))
                continue;

            const size_t lineSize = line.size();
            const size_t curPos = !sourceStream.eof() ? static_cast<size_t>(sourceStream.tellg()) - 1 : source.size();
            const size_t startPos = curPos - lineSize;

            line.erase(0, 9); // Remove the #include
            const auto trimCallback = [](const char c)
            {
                return std::isspace(c) || c == '"' || c == '<' || c == '>';
            };

            trimString(line, trimCallback);

            if (line.empty())
            {
                DEBUG_LOG("Empty shader include path", line.c_str());
                return false;
            }

            std::ifstream sourceFile(line, std::ios::binary | std::ios::ate);

            if (!sourceFile.is_open())
            {
                DEBUG_LOG("Invalid shader include path: \"%s\"", line.c_str());
                return false;
            }

            const std::ifstream::pos_type length = sourceFile.tellg();
            sourceFile.seekg(0, std::ios::beg);

            std::string includedShader(length, 0);
            sourceFile.read(includedShader.data(), length);
            sourceFile.close();

            if (!processIncludes(includedShader))
                return false;

            source.replace(startPos, lineSize, includedShader);
        }

        return true;
    }

    GLuint Shader::compileSource(const GLenum shaderType, std::string& source)
    {
        const GLuint shaderId = glCreateShader(shaderType);

        processIncludes(source);
        const char* shaderSource = source.c_str();
        const auto  sourceSize = static_cast<GLint>(source.size());

        glShaderSource(shaderId, 1, &shaderSource, &sourceSize);
        glCompileShader(shaderId);

        int success;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char infoLog[INFO_LOG_SIZE];
            glGetShaderInfoLog(shaderId, INFO_LOG_SIZE, nullptr, infoLog);
            DEBUG_LOG("ERROR::SHADER::%s::COMPILATION_FAILED\n%s", getTokenFromType(shaderType).c_str(), infoLog);
            glDeleteShader(shaderId);
            return 0;
        }

        return shaderId;
    }

    bool Shader::parseSource()
    {
        glDeleteProgram(m_program);
        m_program = 0;

        if (m_source.empty())
            return false;

        m_program = glCreateProgram();

        std::vector<std::string> sources = splitString(m_source, "#shader ", true);

        std::vector<GLuint> shaderIds;
        shaderIds.reserve(sources.size());

        for (std::string& source : sources)
        {
            if (source.empty())
                continue;

            std::istringstream iStrStream(source);
            std::string        token;

            iStrStream >> token;
            const GLuint shaderType = getTypeFromToken(token);

            if (shaderType != GL_INVALID_VALUE)
            {
                std::string firstLine;
                std::getline(iStrStream, firstLine);
                source.erase(0, token.size() + firstLine.size());

                if (const GLuint shaderId = compileSource(shaderType, source))
                {
                    shaderIds.emplace_back(shaderId);
                    glAttachShader(m_program, shaderId);
                }
            }
        }

        const bool isSuccess = link();

        for (const GLuint shaderId : shaderIds)
            glDeleteShader(shaderId);

        return isSuccess;
    }

    bool Shader::link() const
    {
        if (m_program == 0)
            return false;

        int success;
        glLinkProgram(m_program);
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);

        if (!success)
        {
            char infoLog[INFO_LOG_SIZE];
            glGetProgramInfoLog(m_program, INFO_LOG_SIZE, nullptr, infoLog);
            DEBUG_LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
            return false;
        }

        glValidateProgram(m_program);
        glGetProgramiv(m_program, GL_VALIDATE_STATUS, &success);

        if (!success)
        {
            char infoLog[INFO_LOG_SIZE];
            glGetProgramInfoLog(m_program, INFO_LOG_SIZE, nullptr, infoLog);
            DEBUG_LOG("ERROR::SHADER::PROGRAM::VALIDATION_FAILED\n%s", infoLog);
            return false;
        }

        return true;
    }

    GLint Shader::getUniformLocation(const std::string& uniformName)
    {
        const auto& it = m_uniformLocationsCache.find(uniformName);

        if (it != m_uniformLocationsCache.end())
            return it->second;

        return m_uniformLocationsCache[uniformName] = glGetUniformLocation(m_program, uniformName.c_str());
    }
}
