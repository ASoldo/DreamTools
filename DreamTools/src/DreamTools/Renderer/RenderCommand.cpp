#include "dtpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace DreamTools
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}