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

		DreamTools::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		DreamTools::RenderCommand::Clear();

		DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());
		DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 100.0f, 100.0f }, m_CheckerBoardTexture, 10.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		DreamTools::Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, glm::radians(45.0f), { 0.2f, 0.8f, 0.6f, 1.0f });
		DreamTools::Renderer2D::DrawQuad({ -0.5f, -1.5f, 0.0f }, { 1.0f, 1.0f }, { 0.4f, 0.8f, 1.0f, 1.0f });
		DreamTools::Renderer2D::DrawQuad({ -0.5f, -2.5f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.8f, 0.0f, 1.0f });
		
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

		ImGui::Begin("Square Color:");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
		ImGui::End();

		ImGui::Begin("Profiling:");

		ImGui::End();
	}
	void Sandbox2D::OnEvent(DreamTools::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}