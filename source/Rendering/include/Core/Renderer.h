#pragma once
#include "Core/Color.h"

#include "Enums/EBlendFactor.h"
#include "Enums/ECompareAlgorithm.h"
#include "Enums/ECullFace.h"
#include "Enums/ERenderingCapability.h"

#include "LowRenderer/Camera.h"

namespace LibGL::Rendering
{
    class Renderer
    {
    public:
        Renderer()                      = default;
        Renderer(const Renderer& other) = default;
        Renderer(Renderer&& other)      = default;

        Renderer& operator=(const Renderer& other) = default;
        Renderer& operator=(Renderer&& other)      = default;

        ~Renderer() = default;

        /**
         * \brief Sets the renderer's clear color
         * \param color The new clear color
         */
        void setClearColor(const Color& color) const;

        /**
         * \brief Sets the renderer's clear color
         * \param r The clear color's red component
         * \param g The clear color's green component
         * \param b The clear color's blue component
         * \param a The clear color's alpha component
         */
        void setClearColor(float r, float g, float b, float a = 1.f) const;

        /**
         * \brief Clears the screen using the previously set clear color
         * \param colorBuffer Whether the color buffer should be cleared or not
         * \param depthBuffer Whether the depth buffer should be cleared or not
         * \param stencilBuffer Whether the stencil buffer should be cleared or not
         */
        void clear(bool colorBuffer = true, bool depthBuffer = true, bool stencilBuffer = true) const;

        /**
         * \brief Clears the screen using the given camera's clear settings
         * \param camera The camera containing the clear color and
         * cleared bits (color, depth, stencil)
         */
        void clear(const Camera& camera) const;

        /**
         * \brief Enables or disables the given rendering capability
         * \param renderingCapability The target rendering capability
         * \param enable Whether the rendering capability should be enabled or not
         */
        void setCapability(ERenderingCapability renderingCapability, bool enable) const;

        /**
         * \brief Checks if the given rendering capability is enabled
         * \param renderingCapability The rendering capability to enable
         * \return True if the given capability is enabled. False otherwise
         */
        bool hasCapability(ERenderingCapability renderingCapability) const;

        /**
         * \brief Sets the depth buffer's compare algorithm
         * \param algorithm The compare algorithm to use for the depth buffer
         */
        void setDepthAlgorithm(ECompareAlgorithm algorithm) const;

        /**
         * \brief Sets the face(s) to cull when the CULL_FACE capability is enabled
         * \param cullFace The faces to cull
         */
        void setCullFace(ECullFace cullFace) const;

        /**
         * \brief Enables or disables writing into the depth buffer
         * \param enable Whether the depth buffer should be writable or not
         */
        void setDepthWriting(bool enable) const;

        /**
         * \brief Enables or disables writing the given color components into the frame buffer
         * \param enableRed Whether red should be written into the frame buffer on not
         * \param enableGreen Whether green should be written into the frame buffer on not
         * \param enableBlue Whether blue should be written into the frame buffer on not
         * \param enableAlpha Whether alpha should be written into the frame buffer on not
         */
        void setColorWriting(bool enableRed, bool enableGreen, bool enableBlue, bool enableAlpha) const;

        /**
         * \brief Sets the color blending function to use
         * when the BLEND capability is enabled
         * \param sourceFactor The source color's factor
         * \param destinationFactor The destination color's factor
         */
        void setBlendFunc(EBlendFactor sourceFactor, EBlendFactor destinationFactor) const;

        /**
         * \brief Sets the viewport's position and size
         * \param x The viewport's x coordinate
         * \param y The viewport's y coordinate
         * \param width The viewport's width
         * \param height The viewport's height
         */
        void setViewPort(int x, int y, int width, int height) const;
    };
}
