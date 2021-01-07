#include "dtpch.h"

#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "../glm/glm/gtc/matrix_transform.hpp"
#include "../glm/glm/gtc/type_ptr.hpp"

#include "../vendor/GLFW/include/GLFW/glfw3.h"

namespace DreamTools
{
	Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
	{

	}

	void Sandbox2D::OnAttach()
	{
		
	}
	void Sandbox2D::OnDetach()
	{
	}
	void Sandbox2D::OnUpdate(DreamTools::Timestep ts)
	{
		m_CameraController.OnUpdate(ts);

		DreamTools::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		DreamTools::RenderCommand::Clear();

		DreamTools::Renderer2D::BeginScene(m_CameraController.GetCamera());

		DreamTools::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.5f, 0.2f, 1.0f });
		DreamTools::Renderer2D::DrawQuad({ 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.4f, 0.1f, 0.7f, 1.0f });
		DreamTools::Renderer2D::EndScene();
	}
	void Sandbox2D::OnImGuiRender()
	{
		ImGui::Begin("OpenGL Info:");
		ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
		ImGui::Text("Version: %s", glGetString(GL_VERSION));
		ImGui::End();

		ImGui::Begin("Square Color:");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
	void Sandbox2D::OnEvent(DreamTools::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}