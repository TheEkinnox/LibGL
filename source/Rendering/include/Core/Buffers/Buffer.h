#pragma once
#include <cstdint>

namespace LibGL::Rendering
{
	class Buffer
	{
	public:
		Buffer(const Buffer& other) = delete;
		Buffer(Buffer&& other) noexcept;
		virtual ~Buffer();

		Buffer& operator=(const Buffer& other) = delete;
		Buffer& operator=(Buffer&& other) noexcept;

		/**
		 * \brief Binds the buffer to the current context
		 */
		virtual void bind() const = 0;

	protected:
		Buffer() = default;

		uint32_t	m_bufferIndex = 0;
	};
}
