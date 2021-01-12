#pragma once
#include "DreamTools/Renderer/Framebuffer.h"

namespace DreamTools
{
	class OpenGLFrameBuffer : public Framebuffer
	{
	public:
		OpenGLFrameBuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
		FramebufferSpecification m_Specification;

	};
}