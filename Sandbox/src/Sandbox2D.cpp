#include "dtpch.h"

#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "../glm/glm/gtc/matrix_transform.hpp"
#include "../glm/glm/gtc/type_ptr.hpp"

#include "../vendor/GLFW/include/GLFW/glfw3.h"

#include <chrono>

namespace DreamTools
{
	Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void Sandbox2D::OnAttach()
	{
		DT_PROFILE_FUNCTION();

		m_CheckerBoardTexture = DreamTools::Texture2D::Create("assets/textures/DreamToolsCheckerboard.png");
		m_Logo = DreamTools::Texture2D::Create("assets/textures/DreamToolsLogo.png");
		m_SpriteSheet = DreamTools::Texture2D::Create("assets/game/textures/RPGpack.png");


		// Init here
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 3.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };
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

		/*DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());		
			DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, { 20.0f, 20.0f }, m_CheckerBoardTexture, 1.0f);
			DreamTools::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.1f },{ 1.0f, 1.0f }, glm::radians(rotation), m_CheckerBoardTexture, 3.0f);
			DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			DreamTools::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.4f }, { 1.0f, 1.0f }, glm::radians(rotation) + 10.0f, m_Logo, 1.0f, {0.0f, 1.0f, 0.0f, 1.0f});
			DreamTools::Renderer2D::DrawRotatedQuad({ -0.5f, -2.5f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), { 0.2f, 0.8f, 0.6f, 1.0f });
		DreamTools::Renderer2D::EndScene();*/

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
		
		if (DreamTools::Input::IsMouseButtonPressed(DreamTools::Mouse::ButtonLeft))
		{
			auto [x, y] = Input::GetMousePosition();
			auto width = DreamTools::Application::Get().GetWindow().GetWidth();
			auto height = DreamTools::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			//m_Particle.Position = { 0, 0 };
			for (int i = 0; i < 5; i++)
				m_PartycleSystem.Emit(m_Particle);
		}

		DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());
			DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.4f }, { 1.0f, 1.0f }, m_SpriteSheet);
		DreamTools::Renderer2D::EndScene();

		m_PartycleSystem.OnUpdate(ts);
		m_PartycleSystem.OnRender(m_CameraController.GetCamera());
	}
	void Sandbox2D::OnImGuiRender()
	{
		DT_PROFILE_FUNCTION();

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
	}

	void Sandbox2D::OnEvent(DreamTools::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}