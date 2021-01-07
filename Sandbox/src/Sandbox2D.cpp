#include "dtpch.h"

#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "../glm/glm/gtc/matrix_transform.hpp"
#include "../glm/glm/gtc/type_ptr.hpp"

#include "Platform/OpenGL/OpenGLShader.h"
#include "../vendor/GLFW/include/GLFW/glfw3.h"

namespace DreamTools
{
	Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
	{

	}

	void Sandbox2D::OnAttach()
	{
		m_SquareVA = DreamTools::VertexArray::Create();


		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<DreamTools::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(DreamTools::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVertexBuffer->SetLayout({
			{
				DreamTools::ShaderDataType::Float3, "a_Position" }
			});

		m_SquareVA->AddVertexBuffer(squareVertexBuffer);
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		DreamTools::Ref<DreamTools::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(DreamTools::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIndexBuffer);
		m_FlatColorShader = DreamTools::Shader::Create("assets/shaders/FlatColorShader.glsl");
	}
	void Sandbox2D::OnDetach()
	{
	}
	void Sandbox2D::OnUpdate(DreamTools::Timestep ts)
	{
		m_CameraController.OnUpdate(ts);

		DreamTools::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		DreamTools::RenderCommand::Clear();

		DreamTools::Renderer::BeginScene(m_CameraController.GetCamera());

		std::dynamic_pointer_cast<DreamTools::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<DreamTools::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

		DreamTools::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		DreamTools::Renderer::EndScene();
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