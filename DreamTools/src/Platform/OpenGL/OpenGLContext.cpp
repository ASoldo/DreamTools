#include "dtpch.h"
#include "OpenGLContext.h"

#include "../vendor/GLFW/include/GLFW/glfw3.h"
#include "glad/glad.h"

namespace DreamTools
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		DT_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DT_CORE_ASSERT(status, "Failed to initialize GLAD!");

		DT_CORE_INFO("OpenGL Info:");
		DT_CORE_INFO("	+--OpenGL Vendor: {0}", glGetString(GL_VENDOR));
		DT_CORE_INFO("	+--OpenGL Renderer: {0}", glGetString(GL_RENDERER));
		DT_CORE_INFO("	+--OpenGL Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}