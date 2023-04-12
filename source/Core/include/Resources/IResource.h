#pragma once

namespace LibGL::Resources
{
	class IResource
	{
	public:
		/**
		 * \brief Destroys the resource
		 */
		virtual ~IResource() = default;

		/**
		 * \brief Tries to load the resource from the given file
		 * \param fileName The resource's path
		 * \return True if the resource was successfully loaded. False otherwise.
		 */
		virtual bool loadFromFile(const std::string& fileName) = 0;
	};
}
