#pragma once
#include "Enums/EAccessSpecifier.h"

#include <cstddef>

namespace LibGL::Rendering
{
    class ShaderStorageBuffer final
    {
    public:
        explicit ShaderStorageBuffer(EAccessSpecifier accessSpecifier, uint32_t bindIndex = 0);
        ShaderStorageBuffer(const ShaderStorageBuffer& other) = delete;
        ShaderStorageBuffer(ShaderStorageBuffer&& other) noexcept;

        ~ShaderStorageBuffer();

        ShaderStorageBuffer& operator=(const ShaderStorageBuffer& other) = delete;
        ShaderStorageBuffer& operator=(ShaderStorageBuffer&& other) noexcept;

        /**
         * \brief Sets the ssbo's bind index
         */
        void setBindIndex(uint32_t bindIndex);

        /**
         * \brief Binds the ssbo at the given bind index
         */
        void bind(uint32_t bindIndex);

        /**
         * \brief Binds the ssbo to the curren bind index
         */
        void bind() const;

        /**
         * \brief Unbinds the ssbo from the current bind index
         */
        void unbind() const;

        /**
         * \brief Sends the given data block to the buffer
         * \param data The data block to send
         * \param size The data block's size in bytes
         */
        void setRawData(const void* data, size_t size) const;

        /**
         * \brief Sends the given objects array to the buffer
         * \param data The objects array to send
         * \param count The number of elements in the array
         */
        template <typename T>
        void setData(const T* data, size_t count) const;

        /**
         * \brief Sets the buffer's data starting at the given offset
         * \param data The data block to send
         * \param size The data block's size in bytes
         * \param offset The updated data block's offset
         */
        void setRawSubData(const void* data, size_t size, ptrdiff_t offset) const;

        /**
         * \brief Sets the buffer's data starting at the given offset
         * \param data The objects array to send
         * \param count The number of elements in the array
         * \param offset The updated data block's offset
         */
        template <typename T>
        void setSubData(const T* data, size_t count, ptrdiff_t offset) const;

    private:
        uint32_t         m_id;
        uint32_t         m_bindIndex;
        EAccessSpecifier m_accessSpecifier;
    };
}

#include "Core/Buffers/ShaderStorageBuffer.inl"
