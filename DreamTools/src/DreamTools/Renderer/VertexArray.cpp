#include "dtpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "../../Platform/OpenGL/OpenGLVertexArray.h"


namespace DreamTools
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    DT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}

		DT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}