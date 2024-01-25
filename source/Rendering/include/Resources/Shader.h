#pragma once
#include <Matrix.h>
#include <string>
#include <Vector.h>

#include <Resources/IResource.h>

namespace LibGL::Rendering::Resources
{
    class Shader final : public LibGL::Resources::IResource
    {
    public:
        using IResource::load;

        Shader() = default;
        Shader(const Shader& other);
        Shader(Shader&& other) noexcept;
        ~Shader() override;

        Shader& operator=(const Shader& other);
        Shader& operator=(Shader&& other) noexcept;

        /**
         * \brief Loads the source code of a shader from a given file
         * \param fileName The path of the source file to load
         * \return True if the shader was successfully loaded. False otherwise.
         */
        bool load(const char* fileName) override;

        /**
         * \brief Initializes the shader
         * \return True on success. False otherwise
         */
        bool init() override;

        /**
         * \brief Uses the shader program.\n
         * IMPORTANT: the shader program MUST have been linked
         */
        void use() const;

        /**
         * \brief Unbinds the current shader from the OpenGL context
         */
        static void unbind();

        /**
         * \brief Sets the value of the int uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformInt(const std::string& name, int value);

        /**
         * \brief Sets the value of the float uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformFloat(const std::string& name, float value);

        /**
         * \brief Sets the value of the Vector2 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformVec2(const std::string& name, const LibMath::Vector2& value);

        /**
         * \brief Sets the value of the Vector3 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformVec3(const std::string& name, const LibMath::Vector3& value);

        /**
         * \brief Sets the value of the Vector4 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformVec4(const std::string& name, const LibMath::Vector4& value);

        /**
         * \brief Sets the value of the Matrix4 uniform with the given name
         * \param name The name of the uniform
         * \param value The value of the uniform
         */
        void setUniformMat4(const std::string& name, const LibMath::Matrix4& value);

        /**
         * \brief Gets the value of the int uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        int getUniformInt(const std::string& name);

        /**
         * \brief Gets the value of the float uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        float getUniformFloat(const std::string& name);

        /**
         * \brief Gets the value of the Vector2 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        LibMath::Vector2 getUniformVec2(const std::string& name);

        /**
         * \brief Gets the value of the Vector3 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        LibMath::Vector3 getUniformVec3(const std::string& name);

        /**
         * \brief Gets the value of the Vector4 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        LibMath::Vector4 getUniformVec4(const std::string& name);

        /**
         * \brief Gets the value of the Matrix4 uniform with the given name
         * \param name The name of the uniform
         * \return The value of the uniform
         */
        LibMath::Matrix4 getUniformMat4(const std::string& name);

    private:
        std::unordered_map<std::string, int> m_uniformLocationsCache;
        std::string                          m_source;
        uint32_t                             m_program = 0;

        static constexpr int INFO_LOG_SIZE = 512;

        /**
         * \brief Converts a shader type enum value to its corresponding token string
         * \param shaderType The target shader type
         * \return The token string corresponding to the given shader type
         */
        static std::string getTokenFromType(uint32_t shaderType);

        /**
         * \brief Converts a shader type token string to its corresponding enum value
         * \param shaderType The target shader type
         * \return The type enum value corresponding to the given shader type token
         */
        static uint32_t getTypeFromToken(const std::string& shaderType);

        /**
         * \brief Processes includes for the given shader source
         * \param source The shader source for which includes should be processed
         * \return True on success. False otherwise
         */
        static bool processIncludes(std::string& source);

        /**
         * \brief Compiles the given shader source
         * \param shaderType The type of shader to compile
         * \param source The source of the shader to compile
         * \return The compiled shader's handle
         */
        static uint32_t compileSource(uint32_t shaderType, std::string& source);

        /**
         * \brief Parses the shader's source and create the appropriate shader types
         * \return True if at least one type of shader was extracted. False otherwise
         */
        bool parseSource();

        /**
         * \brief Links the shader program.\n
         * \return True if the shader is linked successfully. False otherwise
         */
        bool link() const;

        /**
         * \brief Gets the location of a given uniform variable
         * in the current shader program
         * \param uniformName The searched uniform variable's name
         * \return The location of the searched uniform variable
         */
        int getUniformLocation(const std::string& uniformName);
    };
}
