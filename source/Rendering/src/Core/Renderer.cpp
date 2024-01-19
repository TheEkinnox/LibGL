#include "Core/Renderer.h"

#include <glad/glad.h>

namespace LibGL::Rendering
{
	void Renderer::setClearColor(const Color& color) const
	{
		setClearColor(color.m_r, color.m_g, color.m_b, color.m_a);
	}

	void Renderer::setClearColor(const float r, const float g, const float b,
		const float a) const
	{
		glClearColor(r, g, b, a);
	}

	void Renderer::clear(const bool colorBuffer, const bool depthBuffer,
		const bool stencilBuffer) const
	{
		glClear
		(
			(colorBuffer ? GL_COLOR_BUFFER_BIT : 0) |
			(depthBuffer ? GL_DEPTH_BUFFER_BIT : 0) |
			(stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
		);
	}

	void Renderer::clear(const Camera& camera) const
	{
		Color currentClearColor;
		glGetFloatv(GL_COLOR_CLEAR_VALUE, &currentClearColor.m_r);

		setClearColor(camera.getClearColor());
		clear(camera.getClearColorBuffer(), camera.getClearDepthBuffer(), camera.getClearStencilBuffer());

		setClearColor(currentClearColor);
	}

	void Renderer::setCapability(const ERenderingCapability renderingCapability,
		const bool enable) const
	{
		(enable ? glEnable : glDisable)(static_cast<GLenum>(renderingCapability));
	}

	bool Renderer::hasCapability(const ERenderingCapability renderingCapability) const
	{
		return glIsEnabled(static_cast<GLenum>(renderingCapability));
	}

	void Renderer::setDepthAlgorithm(const ECompareAlgorithm algorithm) const
	{
		glDepthFunc(static_cast<GLenum>(algorithm));
	}

	void Renderer::setCullFace(const ECullFace cullFace) const
	{
		glCullFace(static_cast<GLenum>(cullFace));
	}

	void Renderer::setDepthWriting(const bool enable) const
	{
		glDepthMask(enable);
	}

	void Renderer::setColorWriting(const bool enableRed, const bool enableGreen,
		const bool enableBlue, const bool enableAlpha) const
	{
		glColorMask(enableRed, enableGreen, enableBlue, enableAlpha);
	}

	void Renderer::setBlendFunc(EBlendFactor sourceFactor, EBlendFactor destinationFactor) const
	{
		glBlendFunc(static_cast<GLenum>(sourceFactor),
			static_cast<GLenum>(destinationFactor));
	}

	void Renderer::setViewPort(const int x, const int y,
		const int width, const int height) const
	{
		glViewport(x, y, width, height);
	}
}
