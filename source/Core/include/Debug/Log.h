#pragma once

#include <filesystem>
#include <fstream>

#define DEBUG_LOG(format, ...) My::Debug::Log::DebugLog(__FILE__, __LINE__, format, ##__VA_ARGS__)

namespace My::Debug
{
	class Log
	{
	public:
		~Log();

		static void			OpenFile(std::filesystem::path const& filePath);

		template			<class ... Args>
		static std::string	Format(const char* format, Args ... args);

		template <typename ... Args>
		static void			Print(const char* format, Args ... args);

		template			<typename ... Args>
		static void			DebugLog(const char* file, size_t line, const char* format, Args ... args);

	private:
		Log() = default;

		static Log&			GetInstance();
		static void			RemoveInstance();

		std::ofstream		m_file;
		inline static Log*	m_instance = nullptr;
	};
}

#include "Debug/Log.inl"
