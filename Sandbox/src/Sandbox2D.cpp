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

		m_CheckerBoardTexture = (DreamTools::Texture2D::Create("assets/textures/DreamToolsCheckerboard.png"));
	}
	void Sandbox2D::OnDetach()
	{
		DT_PROFILE_FUNCTION();

	}
	void Sandbox2D::OnUpdate(DreamTools::Timestep ts)
	{
		DT_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(ts);

		DreamTools::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		DreamTools::RenderCommand::Clear();

		static float rotation = 0.0f;
		rotation += ts * 20.0f;

		DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());
		DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f },  m_CheckerBoardTexture, 1.0f);
		DreamTools::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f, 0.1f },{ 1.0f, 1.0f }, rotation, m_CheckerBoardTexture, 20.0f);

		DreamTools::Renderer2D::DrawRotatedQuad({ -0.5f, -2.5f, 0.0f }, { 1.0f, 1.0f }, rotation, { 0.2f, 0.8f, 0.6f, 1.0f });

		DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		DreamTools::Renderer2D::DrawQuad({ 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		
		DreamTools::Renderer2D::EndScene();
	}
	void Sandbox2D::OnImGuiRender()
	{
		DT_PROFILE_FUNCTION();

		ImGui::Begin("OpenGL Info:");
		ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
		ImGui::Text("Version: %s", glGetString(GL_VERSION));
		ImGui::End();
	}

	void Sandbox2D::OnEvent(DreamTools::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}