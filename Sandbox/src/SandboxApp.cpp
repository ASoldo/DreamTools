#include <DreamTools.h>
#include "DreamTools/ImGui/ImGuiLayer.h"

#include "imgui/imgui.h"

#include "../vendor/GLFW/include/GLFW/glfw3.h"

//OpenGL Matehmatics Demo
//#include <glm/vec3.hpp> // glm::vec3
//#include <glm/vec4.hpp> // glm::vec4
//#include <glm/mat4x4.hpp> // glm::mat4
//#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
//{
//	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
//	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
//	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
//	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
//	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
//	return Projection * View * Model;
//}

class ExampleLayer : public DreamTools::Layer
{
public:
	ExampleLayer() : Layer("Example") , m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		//OpenGL Matehmatics Demo
		//auto cam = camera(5.0f, { 0.5f, 0.5f });

		m_VertexArray.reset(DreamTools::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		std::shared_ptr<DreamTools::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(DreamTools::VertexBuffer::Create(vertices, sizeof(vertices)));

		DreamTools::BufferLayout layout =
		{
			{ DreamTools::ShaderDataType::Float3, "a_Position" },
			{ DreamTools::ShaderDataType::Float4, "a_Color" },
		};


		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<DreamTools::IndexBuffer> indexBuffer;
		indexBuffer.reset(DreamTools::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray.reset(DreamTools::VertexArray::Create());


		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,
		};

		std::shared_ptr<DreamTools::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(DreamTools::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		/*BufferLayout squareVertexBufferLayout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};*/

		squareVertexBuffer->SetLayout({ { DreamTools::ShaderDataType::Float3, "a_Position" } });
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<DreamTools::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(DreamTools::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		//Shader Source code (Vertex)
		std::string vertexSrc = R"(
			#version 330 core

			//vec3 is 12 bits
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
	
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
				
			}
		)";

		//Shader Source code (Fragment)
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			
			void main()
			{
				//color = vec4(1.0, 0.5, 0.65, 1.0);
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";


		m_Shader.reset(new DreamTools::Shader(vertexSrc, fragmentSrc));

		//Shader2 start
		//Shader Source code (Vertex)
		std::string blueShaderVertexSrc = R"(
			#version 330 core

			//vec3 is 12 bits
			layout(location = 0) in vec3 a_Position;
	
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
				
			}
		)";

		//Shader Source code (Fragment)
		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				//color = vec4(1.0, 0.5, 0.65, 1.0);
				color = vec4(0.1, 0.2, 0.3, 1.0);
			}
		)";


		m_BlueShader.reset(new DreamTools::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
		//Shader2 End

		//Index Buffer
		//Shader
	}
	void OnUpdate() override
	{
		//DT_CORE_INFO("ExampleLayer::OnUpdate");

		/*if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Tab))
		{
			DT_CORE_TRACE("Tab is pressed! (POLL)");
		}*/

		DreamTools::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		DreamTools::RenderCommand::Clear();


		//Set Camera Position
		//m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
		m_Camera.SetRotation(45.0f);

		DreamTools::Renderer::BeginScene(m_Camera);

		DreamTools::Renderer::Submit(m_BlueShader, m_SquareVertexArray);

		DreamTools::Renderer::Submit(m_Shader, m_VertexArray);

		DreamTools::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("OpenGL Info:");
		ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
		ImGui::Text("Version: %s", glGetString(GL_VERSION));
		ImGui::End();
	}

	void OnEvent(DreamTools::Event& event) override
	{
		//OLD
		/*if (event.GetEventType() == DreamTools::EventType::KeyPressed)
		{
			DreamTools::KeyPressedEvent& e = (DreamTools::KeyPressedEvent&)event;
			if (e.GetKeyCode() == DreamTools::Key::Tab)
			{
				DT_CORE_TRACE("{0} is pressed! (EVENT)", (char)e.GetKeyCode());
			}

			DT_CORE_TRACE("{0}", (char)e.GetKeyCode());
		}*/
		//DT_CORE_TRACE("{0}", event);
	}

	private:
		std::shared_ptr<DreamTools::Shader> m_Shader;
		std::shared_ptr<DreamTools::VertexArray> m_VertexArray;

		std::shared_ptr<DreamTools::Shader> m_BlueShader;
		std::shared_ptr<DreamTools::VertexArray> m_SquareVertexArray;

		DreamTools::OrthographicCamera m_Camera;
};

class Sandbox : public DreamTools::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

DreamTools::Application* DreamTools::CreateApplication()
{
	return new Sandbox;
}