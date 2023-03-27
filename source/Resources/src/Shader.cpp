#include "Debug/Log.h"
#include "Shader.h"

#include <sstream>

#include "Utility/utility.h"

namespace My::Resources
{
	Shader::Shader(const Shader& other) :
		m_source(other.m_source), m_vertexShader(other.m_vertexShader),
		m_fragmentShader(other.m_fragmentShader), m_program(other.m_program)
	{
	}

	Shader::Shader(Shader&& other) noexcept :
		m_source(std::move(other.m_source)), m_vertexShader(other.m_vertexShader),
		m_fragmentShader(other.m_fragmentShader), m_program(other.m_program)
	{
		other.m_vertexShader = other.m_fragmentShader = other.m_program = 0;
	}

	Shader::~Shader()
	{
		glDeleteShader(m_vertexShader);
		glDeleteShader(m_fragmentShader);
		glDeleteProgram(m_program);
	}

	Shader& Shader::operator=(const Shader& other)
	{
		if (&other == this)
			return *this;

		m_source = other.m_source;
		m_vertexShader = other.m_vertexShader;
		m_fragmentShader = other.m_fragmentShader;
		m_program = other.m_program;

		return *this;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		if (&other == this)
			return *this;

		m_source = other.m_source;
		m_vertexShader = other.m_vertexShader;
		m_fragmentShader = other.m_fragmentShader;
		m_program = other.m_program;

		other.m_vertexShader = other.m_fragmentShader = other.m_program = 0;

		return *this;
	}

	bool Shader::loadFromFile(const std::string& fileName)
	{
		const std::ifstream fs(fileName);

		if (!fs.is_open())
		{
			DEBUG_LOG("Unable to open file at path \"%s\"\n", fileName.c_str());
			return false;
		}

		std::ostringstream stringStream;

		stringStream << fs.rdbuf();

		m_source = stringStream.str();

		return true;
	}

	std::string Shader::getSource(GLenum shaderType)
	{
		const auto sources = Utility::splitString(m_source, "#shader ", true);

		if (sources.size() < 2)
			return m_source;

		std::string typeToken;

		switch (shaderType)
		{
		case GL_VERTEX_SHADER:
			typeToken = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			typeToken = "fragment";
			break;
		default:
			return m_source;
		}

		for (const auto& source : sources)
		{
			std::istringstream isStream(source);
			std::string token;

			isStream >> token;

			if (token == typeToken)
			{
				std::string firstLine;
				std::getline(isStream, firstLine);

				// Return source without the shader type line
				return source.substr(token.size() + firstLine.size());
			}
		}

		return "";
	}

	bool Shader::setVertexShader()
	{
		if (m_vertexShader != 0)
		{
			glDetachShader(m_program, m_vertexShader);
			glDeleteShader(m_vertexShader);
		}

		m_vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const std::string vertexSource = getSource(GL_VERTEX_SHADER);
		const char* shaderSource = vertexSource.c_str();
		const GLint sourceSize = static_cast<GLint>(vertexSource.size());

		glShaderSource(m_vertexShader, 1, &shaderSource, &sourceSize);
		glCompileShader(m_vertexShader);

		int success;
		glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char infoLog[INFO_LOG_SIZE];
			glGetShaderInfoLog(m_vertexShader, INFO_LOG_SIZE, nullptr, infoLog);
			DEBUG_LOG("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
			glDeleteShader(m_vertexShader);
			m_vertexShader = 0;
			return false;
		}

		if (m_program == 0)
			m_program = glCreateProgram();

		glAttachShader(m_program, m_vertexShader);

		return true;
	}

	bool Shader::setFragmentShader()
	{
		if (m_fragmentShader != 0)
		{
			glDetachShader(m_program, m_fragmentShader);
			glDeleteShader(m_fragmentShader);
		}

		m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const std::string fragmentSource = getSource(GL_FRAGMENT_SHADER);
		const char* shaderSource = fragmentSource.c_str();
		const GLint sourceSize = static_cast<GLint>(fragmentSource.size());

		glShaderSource(m_fragmentShader, 1, &shaderSource, &sourceSize);
		glCompileShader(m_fragmentShader);

		int success;
		glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char infoLog[INFO_LOG_SIZE];
			glGetShaderInfoLog(m_fragmentShader, INFO_LOG_SIZE, nullptr, infoLog);
			DEBUG_LOG("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
			glDeleteShader(m_fragmentShader);
			m_fragmentShader = 0;
			return false;
		}

		if (m_program == 0)
			m_program = glCreateProgram();

		glAttachShader(m_program, m_fragmentShader);

		return true;
	}

	bool Shader::link() const
	{
		if (m_program == 0)
			return false;

		glLinkProgram(m_program);

		int success;
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);

		if (!success)
		{
			char infoLog[INFO_LOG_SIZE];
			glGetProgramInfoLog(m_program, INFO_LOG_SIZE, nullptr, infoLog);
			DEBUG_LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
			return false;
		}

		return true;
	}

	void Shader::use() const
	{
		glUseProgram(m_program);
	}

	GLint Shader::getUniformLocation(const std::string& uniformName) const
	{
		return glGetUniformLocation(m_program, uniformName.c_str());
	}
}
