#pragma once
#include "DreamTools/Core/Core.h"

namespace DreamTools
{
	struct FramebufferSpecification
	{
		uint32_t Width;
		uint32_t Height;
		//FramebufferFormat Format = 
		uint32_t Samples = 1;

		bool SwapChaintarget = false;
	};

	class Framebuffer
	{
	public:

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		//virtual FramebufferSpecification& GetSpecification() = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;
		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}