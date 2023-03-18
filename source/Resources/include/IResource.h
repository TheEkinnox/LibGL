#pragma once

namespace My::Resources
{
	class IResource
	{
	public:
		virtual			~IResource() = default;

		virtual bool	LoadFromFile(const std::string& fileName) = 0;
	};
}
