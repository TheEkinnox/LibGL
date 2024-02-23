#pragma once
#include "Core/Buffers/Buffer.h"
#include "Enums/EAccessSpecifier.h"

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
         * \param size The data block's size in bytes
         */
        void sendBlocks(const void* data, size_t size) const;

        /**
         * \brief Sends the given objects array to the buffer
         * \param data The objects array to send
         * \param count The number of elements in the array
         */
        template <typename T>
        void sendBlocks(const T* data, size_t count) const;

    private:
        uint32_t m_bindingPoint = 0;
    };
}

#include "Core/Buffers/ShaderStorageBuffer.inl"
