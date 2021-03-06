#include "dtpch.h"
#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include "../glm/glm/gtc/matrix_transform.hpp"
#include "../glm/glm/gtc/type_ptr.hpp"
#include "../vendor/GLFW/include/GLFW/glfw3.h"
#include <chrono>

namespace DreamTools
{
	static uint32_t s_MapWidth = 6;
	
	static const char* s_MapTiles = 
	"WWWWWW"
	"WWWWWW"
	"DDWDWD"
	"DDDDDD"
	"DDDDDD"
	"DDDDDD";

	Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void Sandbox2D::OnAttach()
	{
		DT_PROFILE_FUNCTION();

		m_CheckerBoardTexture = DreamTools::Texture2D::Create("assets/textures/DreamToolsCheckerboard.png");
		/*m_Logo = DreamTools::Texture2D::Create("assets/textures/DreamToolsLogo.png");
		m_SpriteSheet = DreamTools::Texture2D::Create("assets/game/textures/RPGpack.png");

		m_TextureStairs = DreamTools::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0,11 }, { 128,128 });

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

		s_TextureMap['D'] = DreamTools::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128,128 }, { 1,1 });
		s_TextureMap['W'] = DreamTools::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128,128 }, { 1,1 });*/

		m_CameraController.SetZoomLevel(5.0f);
	}
	void Sandbox2D::OnDetach()
	{
		DT_PROFILE_FUNCTION();

	}
	void Sandbox2D::OnUpdate(DreamTools::Timestep ts)
	{
		DT_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(ts);

		DreamTools::Renderer2D::ResetStats();

		DreamTools::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		DreamTools::RenderCommand::Clear();

		static float rotation = 0.0f;
		rotation += ts * 20.0f;

		DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());		
			DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, { 20.0f, 20.0f }, m_CheckerBoardTexture, 1.0f);
			/*DreamTools::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.1f },{ 1.0f, 1.0f }, glm::radians(rotation), m_CheckerBoardTexture, 3.0f);
			DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			DreamTools::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.4f }, { 1.0f, 1.0f }, glm::radians(rotation) + 10.0f, m_Logo, 1.0f, {0.0f, 1.0f, 0.0f, 1.0f});
			DreamTools::Renderer2D::DrawRotatedQuad({ -0.5f, -2.5f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), { 0.2f, 0.8f, 0.6f, 1.0f });*/
		DreamTools::Renderer2D::EndScene();

		//DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//for (float y = -5.0f; y < 5.0f; y += 0.5f)
		//{
		//	for (float x = -5.0f; x < 5.0f; x += 0.5f)
		//	{
		//		glm::vec4 color = { (x + 5.0f) / 10, 0.4f, (y + 5.0f) / 10, 0.5f };
		//		DreamTools::Renderer2D::DrawQuad({ x, y, -0.1f }, { 0.45f, 0.45f }, color);
		//	}
		//}
		//DreamTools::Renderer2D::EndScene();
		
		DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (uint32_t y = 0; y < m_MapHeight; y++)
		{
			for (uint32_t x = 0; x < m_MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y * m_MapWidth];
				DreamTools::Ref<DreamTools::SubTexture2D> texture;
				if (s_TextureMap.find(tileType) != s_TextureMap.end())
				{
					texture = s_TextureMap[tileType];
				}
				else
				{
					texture = m_TextureStairs;
				}
				DreamTools::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight / 2.0f - y , 0.4f }, { 1.0f, 1.0f }, texture);
			}
		}
		DreamTools::Renderer2D::EndScene();
		

		m_PartycleSystem.OnUpdate(ts);
		m_PartycleSystem.OnRender(m_CameraController.GetCamera());
	}
	void Sandbox2D::OnImGuiRender()
	{
		DT_PROFILE_FUNCTION();

		//CUSTOM UI START

		ImGui::Begin("OpenGL Info:");
		ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
		ImGui::Text("Version: %s", glGetString(GL_VERSION));
		ImGui::End();

		auto stats = DreamTools::Renderer2D::GetStats();
		ImGui::Begin("Renderer2D Stats:");
		ImGui::Text("DrawCalls: %d", stats.DrawCalls);
		ImGui::Text("QuadCount: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::Begin("Scene:");
			uint32_t textureID = m_CheckerBoardTexture->GetRendererID();
			ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f }, ImVec2{ 0,1 }, ImVec2{1,0});
		ImGui::End();

	}

	void Sandbox2D::OnEvent(DreamTools::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}