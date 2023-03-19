#pragma once

namespace My::Resources
{
	class IResource
	{
	public:
		virtual			~IResource() = default;

		virtual bool	loadFromFile(const std::string& fileName) = 0;
	};
}
