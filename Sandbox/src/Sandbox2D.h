#pragma once
#include "DreamTools.h"

namespace DreamTools
{
	class Sandbox2D : public DreamTools::Layer
	{
	public:
		Sandbox2D();
		virtual ~Sandbox2D() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(DreamTools::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(DreamTools::Event& e) override;

	private:
		DreamTools::OrthographicCameraController m_CameraController;
		DreamTools::Ref<DreamTools::VertexArray> m_SquareVA;
		DreamTools::Ref<DreamTools::Shader> m_FlatColorShader;

		DreamTools::Ref<DreamTools::Texture2D> m_CheckerBoardTexture;
		DreamTools::Ref<DreamTools::Texture2D> m_Logo;

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};

		std::vector<ProfileResult> m_ProfileResults;
		
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.4f, 1.0f };

	};
}