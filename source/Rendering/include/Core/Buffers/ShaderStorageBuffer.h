#pragma once
#include "Enums/EAccessSpecifier.h"
#include "Core/Buffers/Buffer.h"

namespace LibGL::Rendering
{
	class ShaderStorageBuffer : public Buffer
	{
	public:
		ShaderStorageBuffer() = default;
		explicit ShaderStorageBuffer(EAccessSpecifier accessSpecifier);

		/**
		 * \brief Sets the ssbo's binding point
		 */
		void setBindingPoint(uint32_t bindingPoint);

		/**
		 * \brief Binds the ssbo at the given binding point
		 */
		void bind(uint32_t bindingPoint);

		/**
		 * \brief Binds the ssbo to the current binding point
		 */
		void bind() const override;

		/**
		 * \brief Unbinds the ssbo from the current binding point
		 */
		void unbind();

		/**
		 * \brief Sends the data block to the buffer
		 * \param data The data block to send
		 * \param count The number of elements in the block
		 */
		template<typename T>
		void sendBlocks(T* data, size_t count) const;

	private:
		uint32_t				m_bindingPoint = 0;

		void sendBlocks(const void* data, size_t blockSize) const;
	};
}

#include "Core/Buffers/ShaderStorageBuffer.inl"
