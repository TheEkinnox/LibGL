#pragma once
#include <any>
#include <unordered_map>

#define LGL_SERVICE(Type) LibGL::ServiceLocator::get<Type>()

namespace LibGL
{
	class ServiceLocator
	{
	public:
		template<typename T>
		static void	provide(T& p_service)
		{
			s_services[typeid(T).hash_code()] = std::any(&p_service);
		}

		template<typename T>
		static T& get()
		{
			return *std::any_cast<T*>(s_services[typeid(T).hash_code()]);
		}

	private:
		inline static std::unordered_map<size_t, std::any>	s_services;
	};
}
