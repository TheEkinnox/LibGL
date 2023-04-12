#pragma once
#include <string>
#include <Resources/IResource.h>

#include "Vector.h"
#include "Matrix.h"

namespace LibGL::Resources
{
	class Shader : public IResource
	{
	public:
		Shader() = default;
		Shader(const Shader& other);
		Shader(Shader&& other) noexcept;
		~Shader() override;

		Shader& operator=(const Shader& other);
		Shader& operator=(Shader&& other) noexcept;

		/**
		 * \brief Loads the source code of a shader from a given file
		 * \param fileName The path of the source file to load
		 * \return True if the source code could be loaded. False otherwise.
		 */
		bool loadFromFile(const std::string& fileName) override;

		/**
		 * \brief Compiles the vertex shader from the current source
		 * and attaches it to the shader program.
		 * \return True if the vertex shader could be compiled. False otherwise.
		 */
		bool setVertexShader();

		/**
		 * \brief Compiles the fragment shader from the current source
		 * and attaches it to the shader program.
		 * \return True if the fragment shader could be compiled. False otherwise.
		 */
		bool setFragmentShader();

		/**
		 * \brief Links the shader program.\n
		 * IMPORTANT: setVertexShader and/or setFragmentShader MUST have been called
		 * \return True if the shader is linked successfully. False otherwise
		 */
		bool link() const;

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
		void setUniformInt(const std::string& name, int value) const;

		/**
		 * \brief Sets the value of the float uniform with the given name
		 * \param name The name of the uniform
		 * \param value The value of the uniform
		 */
		void setUniformFloat(const std::string& name, float value) const;

		/**
		 * \brief Sets the value of the Vector2 uniform with the given name
		 * \param name The name of the uniform
		 * \param value The value of the uniform
		 */
		void setUniformVec2(const std::string& name, const LibMath::Vector2& value) const;

		/**
		 * \brief Sets the value of the Vector3 uniform with the given name
		 * \param name The name of the uniform
		 * \param value The value of the uniform
		 */
		void setUniformVec3(const std::string& name, const LibMath::Vector3& value) const;

		/**
		 * \brief Sets the value of the Vector4 uniform with the given name
		 * \param name The name of the uniform
		 * \param value The value of the uniform
		 */
		void setUniformVec4(const std::string& name, const LibMath::Vector4& value) const;

		/**
		 * \brief Sets the value of the Matrix4 uniform with the given name
		 * \param name The name of the uniform
		 * \param value The value of the uniform
		 */
		void setUniformMat4(const std::string& name, const LibMath::Matrix4& value) const;

		/**
		 * \brief Gets the value of the int uniform with the given name
		 * \param name The name of the uniform
		 * \return The value of the uniform
		 */
		int getUniformInt(const std::string& name) const;

		/**
		 * \brief Gets the value of the float uniform with the given name
		 * \param name The name of the uniform
		 * \return The value of the uniform
		 */
		float getUniformFloat(const std::string& name) const;

		/**
		 * \brief Gets the value of the Vector2 uniform with the given name
		 * \param name The name of the uniform
		 * \return The value of the uniform
		 */
		LibMath::Vector2 getUniformVec2(const std::string& name) const;

		/**
		 * \brief Gets the value of the Vector3 uniform with the given name
		 * \param name The name of the uniform
		 * \return The value of the uniform
		 */
		LibMath::Vector3 getUniformVec3(const std::string& name) const;

		/**
		 * \brief Gets the value of the Vector4 uniform with the given name
		 * \param name The name of the uniform
		 * \return The value of the uniform
		 */
		LibMath::Vector4 getUniformVec4(const std::string& name) const;

		/**
		 * \brief Gets the value of the Matrix4 uniform with the given name
		 * \param name The name of the uniform
		 * \return The value of the uniform
		 */
		LibMath::Matrix4 getUniformMat4(const std::string& name) const;

	private:
		std::string				m_source;
		uint32_t				m_vertexShader = 0;
		uint32_t				m_fragmentShader = 0;
		uint32_t				m_program = 0;

		static constexpr int	INFO_LOG_SIZE = 512;

		/**
		 * \brief Extracts the code for a given shader type from the raw source
		 * \param shaderType The shader type for which source should be extracted
		 * \return The source code for the given shader type
		 */
		std::string getSource(uint32_t shaderType);

		/**
		 * \brief Gets the location of a given uniform variable
		 * in the current shader program
		 * \param uniformName The searched uniform variable's name
		 * \return The location of the searched uniform variable
		 */
		int getUniformLocation(const std::string& uniformName) const;
	};
}
