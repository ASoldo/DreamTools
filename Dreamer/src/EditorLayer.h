#pragma once
#include "DreamTools.h"
#include "Panels/SceneHierarchyPanel.h"

namespace DreamTools
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		
	private:
		OrthographicCameraController m_CameraController;
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;

		Ref<Texture2D> m_CheckerBoardTexture;
		Ref<Texture2D> m_Logo;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_TextureStairs;
		Ref<Framebuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCameraEntity;

		bool m_PrimaryCamera = true;

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};

		std::vector<ProfileResult> m_ProfileResults;
		
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.4f, 1.0f };

		bool m_ViewPortFocused = false;
		bool m_ViewPortHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		uint32_t m_MapWidth;
		uint32_t m_MapHeight;
		std::unordered_map<char, DreamTools::Ref<DreamTools::SubTexture2D>> s_TextureMap;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

	};
}