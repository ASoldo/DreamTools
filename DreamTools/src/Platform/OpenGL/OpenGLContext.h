#pragma once
#include "DreamTools/Renderer/GraphicsContext.h"

struct GLFWwindow;
namespace DreamTools
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;

	};
}