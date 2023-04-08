#pragma once
#include <unordered_map>

#include "Debug/Assertion.h"

#define LGL_SERVICE(Type) LibGL::ServiceLocator::get<Type>()

namespace LibGL
{
	class ServiceLocator
	{
	public:
		template<typename T>
		static void	provide(T& p_service)
		{
			s_services[typeid(T).hash_code()] = &p_service;
		}

		template<typename T>
		static T& get()
		{
			ASSERT(s_services.contains(typeid(T).hash_code()));
			return *static_cast<T*>(s_services[typeid(T).hash_code()]);
		}

	private:
		inline static std::unordered_map<size_t, void*>	s_services;
	};
}
