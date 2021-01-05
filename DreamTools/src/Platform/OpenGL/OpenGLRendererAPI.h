#pragma once
#include "DreamTools/Renderer/RendererAPI.h"

namespace DreamTools
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const DreamTools::Ref<VertexArray>& vertexArray) override;
	};
}
