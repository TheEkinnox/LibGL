#pragma once

namespace My::Resources
{
	class IResource
	{
	public:
		virtual ~IResource() = 0;

	protected:
		virtual void LoadFromFile(const std::string& fileName) = 0;
	};
}
